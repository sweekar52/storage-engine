#include "../include/storage_engine.h"
#include <filesystem>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

// Constructor
StorageEngine::StorageEngine(const std::string& db_path) 
    : db_path(db_path), sstable_counter(0) {
    
    // Create database directory if it doesn't exist
    fs::create_directories(db_path);
    
    // Create fresh MemTable
    active_memtable = std::make_unique<MemTable>();
    
    // Load existing SSTables from disk
    load_existing_sstables();
    
    std::cout << "StorageEngine initialized at: " << db_path << std::endl;
    std::cout << "Loaded " << sstables.size() << " existing SSTables" << std::endl;
}

// Destructor - flush any remaining data
StorageEngine::~StorageEngine() {
    if (active_memtable->count() > 0) {
        std::cout << "Flushing remaining MemTable on shutdown..." << std::endl;
        flush_memtable();
    }
}

// Put operation with automatic flushing
void StorageEngine::put(const std::string& key, const std::string& value) {
    // Check if MemTable is full - THIS IS WHERE OVERFLOW IS HANDLED!
    if (active_memtable->is_full()) {
        std::cout << "MemTable full! Flushing to disk..." << std::endl;
        flush_memtable();
    }
    
    // Insert into MemTable
    active_memtable->put(key, value);
}

// Get operation - search all layers
std::optional<std::string> StorageEngine::get(const std::string& key) {
    // Step 1: Check MemTable first (most recent)
    auto result = active_memtable->get(key);
    if (result) {
        return result;  // Found in memory!
    }
    
    // Step 2: Check SSTables from newest to oldest
    // sstables are stored oldest-first, so iterate backwards
    for (auto it = sstables.rbegin(); it != sstables.rend(); ++it) {
        result = (*it)->get(key);
        if (result) {
            return result;  // Found in SSTable!
        }
    }
    
    // Not found anywhere
    return std::nullopt;
}

// Flush MemTable to disk as new SSTable
void StorageEngine::flush_memtable() {
    // Generate filename: data_001.sst, data_002.sst, etc.
    std::string filename = generate_sstable_filename();
    
    // Create new SSTable and write MemTable to it
    auto new_sstable = std::make_unique<SSTable>(filename);
    new_sstable->write_from_memtable(*active_memtable);
    
    // Add to our list of SSTables
    sstables.push_back(std::move(new_sstable));
    
    // Create fresh MemTable
    active_memtable = std::make_unique<MemTable>();
}

// Load existing SSTable files from disk
void StorageEngine::load_existing_sstables() {
    if (!fs::exists(db_path)) {
        return;
    }
    
    // Find all .sst files
    std::vector<std::string> sst_files;
    for (const auto& entry : fs::directory_iterator(db_path)) {
        if (entry.path().extension() == ".sst") {
            sst_files.push_back(entry.path().string());
        }
    }
    
    // Sort by filename (data_001.sst, data_002.sst, ...)
    std::sort(sst_files.begin(), sst_files.end());
    
    // Load each SSTable
    for (const auto& filename : sst_files) {
        auto sst = std::make_unique<SSTable>(filename);
        sstables.push_back(std::move(sst));
        sstable_counter++;
    }
}

// Generate next SSTable filename
std::string StorageEngine::generate_sstable_filename() {
    sstable_counter++;
    
    // Format: data_001.sst, data_002.sst, etc.
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s/data_%03d.sst", 
             db_path.c_str(), sstable_counter);
    
    return std::string(buffer);
}