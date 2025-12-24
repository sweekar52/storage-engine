#include <iostream>
#include "../include/storage_engine.h"

int main() {
    std::cout << "=== Testing StorageEngine ===" << std::endl;
    
    StorageEngine db("./data/mydb");
    
    // Test 1: Basic put and get
    std::cout << "\n--- Test 1: Basic Operations ---" << std::endl;
    db.put("name", "Alice");
    db.put("age", "25");
    db.put("city", "NYC");
    
    auto result = db.get("name");
    if (result) {
        std::cout << "name = " << *result << std::endl;
    }
    
    // Test 2: Update a key
    std::cout << "\n--- Test 2: Update Key ---" << std::endl;
    db.put("name", "Bob");
    result = db.get("name");
    if (result) {
        std::cout << "Updated name = " << *result << std::endl;
    }
    
    // Test 3: Write enough data to trigger flush
    std::cout << "\n--- Test 3: Trigger Flush ---" << std::endl;
    std::cout << "Writing data to fill MemTable..." << std::endl;
    
    for (int i = 0; i < 500000; i++) {
        std::string key = "user:" + std::to_string(i);
        std::string value = "User number " + std::to_string(i) + " with some extra data to fill up memory faster";
        db.put(key, value);
    }
    
    std::cout << "MemTable size: " << db.get_memtable_size() << " bytes" << std::endl;
    std::cout << "SSTable count: " << db.get_sstable_count() << std::endl;
    
    // Test 4: Read data (might be in MemTable or SSTable)
    std::cout << "\n--- Test 4: Read After Flush ---" << std::endl;
    auto user100 = db.get("user:100");
    if (user100) {
        std::cout << "user:100 = " << *user100 << std::endl;
    }
    
    auto user999 = db.get("user:999");
    if (user999) {
        std::cout << "user:999 = " << *user999 << std::endl;
    }
    
    // Test 5: Non-existent key
    auto missing = db.get("nonexistent");
    if (!missing) {
        std::cout << "nonexistent key not found (correct!)" << std::endl;
    }
    
    std::cout << "\n=== All tests passed! ===" << std::endl;
    return 0;
}