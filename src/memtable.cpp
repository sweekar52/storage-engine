#include "../include/memtable.h"

// Constructor - initialize member variables
MemTable::MemTable() : size_bytes(0) {
    // data is automatically initialized as empty map
}

// Add or update a key-value pair
void MemTable::put(const std::string& key, const std::string& value) {
    // Check if key already exists
    auto it = data.find(key);
    
    if (it != data.end()) {
        // Key exists - updating
        // Subtract old value size, add new value size
        size_bytes -= it->second.size();
        size_bytes += value.size();
        it->second = value;  // Update the value
    } else {
        // New key - add sizes
        size_bytes += key.size() + value.size();
        data[key] = value;
    }
}

// Retrieve a value by key
std::optional<std::string> MemTable::get(const std::string& key) const {
    auto it = data.find(key);
    
    if (it != data.end()) {
        return it->second;  // Found - return the value
    }
    
    return std::nullopt;  // Not found
}

// Check if MemTable has reached size limit
bool MemTable::is_full() const {
    return size_bytes >= MAX_SIZE;
}

// Get current size in bytes
size_t MemTable::get_size() const {
    return size_bytes;
}

// Get number of entries
size_t MemTable::count() const {
    return data.size();
}