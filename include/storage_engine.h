#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include "memtable.h"
#include "sstable.h"
#include <string>
#include <vector>
#include <optional>
#include <memory>

class StorageEngine {
private:
    std::string db_path;                           // e.g., "./mydb"
    std::unique_ptr<MemTable> active_memtable;     // Current MemTable
    std::vector<std::unique_ptr<SSTable>> sstables; // All SSTable files
    int sstable_counter;                           
    
    void flush_memtable();
    void load_existing_sstables();
    std::string generate_sstable_filename();
    
public:
    StorageEngine(const std::string& db_path);
    ~StorageEngine();
    
    void put(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key);
    
    // For testing/debugging
    size_t get_sstable_count() const { return sstables.size(); }
    size_t get_memtable_size() const { return active_memtable->get_size(); }
};

#endif