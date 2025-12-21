#ifndef SSTABLE_H
#define SSTABLE_H

#include <string>
#include <map>
#include <optional>
#include <fstream>

class MemTable;  // Forward declaration

class SSTable {
private:
    std::string filename;
    std::map<std::string, size_t> index;  // key -> file offset
    
    void build_index();
    
public:
    SSTable(const std::string& filename);
    
    // Write a MemTable to disk
    void write_from_memtable(const MemTable& memtable);
    
    // Read a value by key
    std::optional<std::string> get(const std::string& key) const;
    
    // Check if SSTable file exists
    bool exists() const;
    
    // Get filename
    std::string get_filename() const { return filename; }
};

#endif