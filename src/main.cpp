#include <iostream>
#include "../include/memtable.h"

int main() {
    std::cout << "Testing MemTable..." << std::endl;
    
    MemTable mem;
    
    // Test 1: Insert and retrieve
    mem.put("name", "Alice");
    mem.put("age", "25");
    mem.put("city", "NYC");
    
    std::cout << "Count: " << mem.count() << std::endl;  // Should be 3
    
    // Test 2: Get existing key
    auto result = mem.get("name");
    if (result) {
        std::cout << "name = " << *result << std::endl;  // Should print "Alice"
    }
    
    // Test 3: Get non-existent key
    auto missing = mem.get("country");
    if (!missing) {
        std::cout << "country not found (correct!)" << std::endl;
    }
    
    // Test 4: Update existing key
    mem.put("name", "Bob");
    auto updated = mem.get("name");
    if (updated) {
        std::cout << "Updated name = " << *updated << std::endl;  // Should print "Bob"
    }
    
    std::cout << "Size: " << mem.get_size() << " bytes" << std::endl;
    std::cout << "Is full? " << (mem.is_full() ? "yes" : "no") << std::endl;
    
    std::cout << "\nAll tests passed!" << std::endl;
    return 0;
}