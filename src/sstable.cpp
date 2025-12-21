#include "../include/sstable.h"
#include "../include/memtable.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Constructor - loads index if file exists
SSTable::SSTable(const std::string& filename) : filename(filename) {
    if (exists()) {
        build_index();
    }
}

// Check if SSTable file exists on disk
bool SSTable::exists() const {
    std::ifstream file(filename);
    return file.good();
}

// Write MemTable contents to disk as SSTable
void SSTable::write_from_memtable(const MemTable& memtable) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    
    size_t offset = 0;
    
    // Write data block (keys come out sorted from std::map!)
    for (const auto& [key, value] : memtable.data) {
        // Store offset for index
        index[key] = offset;
        
        // Write in format: key=value\n
        std::string line = key + "=" + value + "\n";
        file << line;
        
        offset += line.size();
    }
    
    // Write index separator
    file << "---INDEX---\n";
    
    // Write index block
    for (const auto& [key, off] : index) {
        file << key << "=" << off << "\n";
    }
    
    file.close();
    std::cout << "Flushed MemTable to " << filename << std::endl;
}

// Read value for a given key
std::optional<std::string> SSTable::get(const std::string& key) const {
    // Look up key in index
    auto it = index.find(key);
    if (it == index.end()) {
        return std::nullopt;  // Key not in this SSTable
    }
    
    size_t offset = it->second;
    
    // Open file and seek to offset
    std::ifstream file(filename);
    if (!file.is_open()) {
        return std::nullopt;
    }
    
    file.seekg(offset);
    
    // Read the line: key=value
    std::string line;
    std::getline(file, line);
    
    // Parse: split by '='
    size_t pos = line.find('=');
    if (pos == std::string::npos) {
        return std::nullopt;
    }
    
    std::string value = line.substr(pos + 1);
    return value;
}

// Build in-memory index from SSTable file
void SSTable::build_index() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    
    std::string line;
    bool in_index = false;
    
    while (std::getline(file, line)) {
        if (line == "---INDEX---") {
            in_index = true;
            continue;
        }
        
        if (in_index) {
            // Parse index line: key=offset
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                size_t offset = std::stoull(line.substr(pos + 1));
                index[key] = offset;
            }
        }
    }
    
    file.close();
}