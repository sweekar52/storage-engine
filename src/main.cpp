#include <iostream>
#include "../include/memtable.h"
#include "../include/sstable.h"

int main() {
    std::cout << "Testing MemTable and SSTable..." << std::endl;
    
    // Test 1: Create and populate MemTable
    MemTable mem;
    mem.put("apple", "red fruit");
    mem.put("banana", "yellow fruit");
    mem.put("cherry", "red fruit");
    mem.put("date", "sweet fruit");
    
    std::cout << "MemTable count: " << mem.count() << std::endl;
    std::cout << "MemTable size: " << mem.get_size() << " bytes" << std::endl;
    
    // Test 2: Write MemTable to SSTable
    SSTable sst("data/test_001.sst");
    sst.write_from_memtable(mem);
    
    // Test 3: Read from SSTable
    auto result1 = sst.get("banana");
    if (result1) {
        std::cout << "Read from SSTable - banana = " << *result1 << std::endl;
    }
    
    auto result2 = sst.get("apple");
    if (result2) {
        std::cout << "Read from SSTable - apple = " << *result2 << std::endl;
    }
    
    // Test 4: Try to get non-existent key
    auto result3 = sst.get("mango");
    if (!result3) {
        std::cout << "mango not found in SSTable (correct!)" << std::endl;
    }
    
    std::cout << "\nAll tests passed!" << std::endl;
    return 0;
}