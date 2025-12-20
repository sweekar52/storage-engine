#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <map>
#include <string>
#include <optional>

class MemTable {
private:
    std::map<std::string, std::string> data;
    size_t size_bytes;
    static const size_t MAX_SIZE = 4 * 1024 * 1024;  // 4MB

public:
    MemTable();
    
    void put(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key) const;
    bool is_full() const;
    size_t get_size() const;
    size_t count() const;
};

#endif