#ifndef TASK
#define TASK

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include "config.hpp"
#include "memoryManager.hpp"

using namespace std;

typedef vector<vector<size_t>*> PageTable;

// TaskMap class uses a map-based page table to manage virtual to physical page mappings
class TaskMap {
private:
    string id;  // Unique identifier for the task
    map<size_t, size_t> pageTable;  // Page table mapping virtual pages to physical pages
    MemoryManager* manager;  // Pointer to the memory manager to allocate/deallocate pages
    size_t pageHit = 0;  // Counter for page hits
    size_t pageMiss = 0;  // Counter for page misses
    double executionTime = 0.0;  // Execution time tracking
    size_t pagesAllocated = 0;  // Counter for pages allocated
    size_t invalidVirtualPages = 0;  // Counter for invalid virtual pages

public:
    // Constructor initializes task ID and memory manager pointer
    TaskMap(const string& id, MemoryManager* const &manager)
        : id(id), manager(manager), pageHit(0), pageMiss(0), executionTime(0) {
        // Initialize all virtual pages to -1 (indicating no mapping)
        for (size_t i = 0; i < virtualPages; ++i) {
            pageTable[i] = -1;
        }
        for (int i=0; i<ceil(getPageTableSize()/pageSize); i++){
            manager->allocatePage();
        }
    }

    ~TaskMap() {
        for (const auto& entry : pageTable) {
            if (entry.second != -1) {
                manager->deallocatePage(entry.second);
            }
        }
        pageTable.clear();
    }

    // Requests memory by allocating required pages and returns the number of page hits
    bool requestMemory(size_t logicalAddress, size_t size) {
        size_t pageReq = (size_t) ceil((double) size / pageSize);  // Calculate required pages, rounding up
        for (size_t i = 0; i < pageReq; ++i) {
            auto start = chrono::high_resolution_clock::now();
            size_t virtualPage = (logicalAddress >> p) + i;  // Calculate the virtual page number
            
            // Check if the virtual page is valid
            if (virtualPage >= virtualPages) {
                ++invalidVirtualPages;
                continue;
            }

            // Check if the page is already mapped
            if (pageTable[virtualPage] != -1) {
                ++pageHit;
                continue;
            } else {
                ++pageMiss;
            }

            // Allocate a physical page and update the page table
            size_t physicalPage = manager->allocatePage();
            if (physicalPage != -1) {
                pageTable[virtualPage] = physicalPage;
                ++pagesAllocated;
            } else {
                cerr << "No free physical pages available for task " << id << "\n";
                return 0;
            }
            auto end = chrono::high_resolution_clock::now();
            executionTime += chrono::duration<double>(end - start).count();
        }
        return 1;
    }

    // Returns the size of the page table
    size_t getPageTableSize() const {
        return pageTable.size() * (pteSize1 + pteSize2);
    }

    // Returns the task ID
    const string& getTaskId() const {
        return id;
    }

    // Returns the number of page hits
    size_t getPageHit() const {
        return pageHit;
    }

    // Returns the execution time
    double getExecutionTime() const {
        return executionTime;
    }

    // Returns the number of page misses
    size_t getPageMiss() const {
        return pageMiss;
    }

    // Returns the amount of memory allocated
    size_t memoryAllocated() const {
        return pagesAllocated * pageSize;
    }

    // Returns the number of invalid virtual pages
    size_t getInvalidVirtualPages() const {
        return invalidVirtualPages;
    }

};

// TaskSingle class uses a single-level page table implemented as a vector
class TaskSingle {
private:
    string id;  // Unique identifier for the task
    vector<size_t> pageTable;  // Page table as a vector where each index is a virtual page number
    MemoryManager* manager;  // Pointer to the memory manager to allocate/deallocate pages
    size_t pageHit = 0;  // Counter for page hits
    size_t pageMiss = 0;  // Counter for page misses
    double executionTime = 0.0;  // Execution time tracking
    size_t pagesAllocated = 0;  // Counter for pages allocated
    size_t invalidVirtualPages = 0;  // Counter for invalid virtual pages

public:
    // Constructor initializes task ID, memory manager pointer, and page table size
    TaskSingle(const string& id, MemoryManager* const &manager)
        : id(id), manager(manager), pageTable(virtualPages, -1), pageHit(0), pageMiss(0), executionTime(0)
        {
            for (int i=0; i<ceil(getPageTableSize()/pageSize); i++){
                manager->allocatePage();
            }
        }

    // destructor
    ~TaskSingle() {
        for (size_t entry : pageTable) {
            if (entry != -1) {
                manager->deallocatePage(entry);
            }
        }
        pageTable.clear();
    }

    // Requests memory by allocating required pages
    bool requestMemory(size_t logicalAddress, size_t size) {
        size_t pageReq = (size_t) ceil((double) size / pageSize);
        for (size_t i = 0; i < pageReq; ++i) {
            auto start = chrono::high_resolution_clock::now();
            size_t virtualPage = (logicalAddress >> p) + i; // Calculate the virtual page number

            // Check if the virtual page is valid
            if (virtualPage >= virtualPages) {
                ++invalidVirtualPages;
                continue;
            }

            // Check if the page is already mapped
            if (pageTable[virtualPage] != -1) {
                ++pageHit;
                continue;
            } else {
                ++pageMiss;
            }

            // Allocate a physical page and update the page table
            size_t physicalPage = manager->allocatePage();
            if (physicalPage != -1) {
                pageTable[virtualPage] = physicalPage;
                ++pagesAllocated;
            } else {
                cerr << "No free physical pages available for task " << id << "\n";
                return 0;
            }
            auto end = chrono::high_resolution_clock::now();
            executionTime += chrono::duration<double>(end - start).count();
        }
        return 1;
    }

    // Returns the size of the page table
    size_t getPageTableSize() const {
        return pageTable.size() * pteSize2;
    }

    // Returns the task ID
    const string& getTaskId() const {
        return id;
    }

    // Returns the number of page hits
    size_t getPageHit() const {
        return pageHit;
    }

    // Returns the execution time
    double getExecutionTime() const {
        return executionTime;
    }

    // Returns the number of page misses
    size_t getPageMiss() const {
        return pageMiss;
    }

    // Returns the amount of memory allocated
    size_t memoryAllocated() const {
        return pagesAllocated * pageSize;
    }

    // Returns the number of invalid virtual pages
    size_t getInvalidVirtualPages() const {
        return invalidVirtualPages;
    }

};

// TaskMulti class uses a multi-level page table implemented with vectors of vectors
class TaskMulti {
private:
    string id;  // Unique identifier for the task
    PageTable pageTable1;  // First-level page table
    MemoryManager* manager;  // Pointer to memory manager to allocate/deallocate pages
    size_t pageHits = 0;  // Counter for page hits
    size_t pageMiss = 0;  // Counter for page misses
    double executionTime = 0.0;  // Execution time tracking
    size_t pagesAllocated = 0;  // Counter for pages allocated
    size_t invalidVirtualPages = 0;  // Counter for invalid virtual pages

public:
    // Constructor initializes task ID, memory manager pointer, and the first-level page table size
    TaskMulti(const string& id, MemoryManager* const &manager)
        : id(id), manager(manager), pageTable1(pageTableSize1, nullptr), pageHits(0), pageMiss(0), executionTime(0) {
            for (int i=0; i< ceil((double) getPageTableSize() / pageSize); i++){
                manager->allocatePage();
            }
        }

    // Destructor
    ~TaskMulti() {
        for (auto& entry : pageTable1) {
            if (entry != nullptr) {
                delete entry;
            }
        }
        pageTable1.clear();
    }

    // Requests memory by allocating required pages using a two-level page table structure
    bool requestMemory(size_t logicalAddress, size_t size) {
        size_t pageReq = (size_t) ceil((double) size / pageSize);
        for (size_t i = 0; i < pageReq; ++i) {
            auto start = chrono::high_resolution_clock::now();
            size_t virtualPage = (logicalAddress >> p) + i; // Calculate the virtual page number

            // Check if the virtual page is valid
            if (virtualPage >= virtualPages) {
                ++invalidVirtualPages;
                continue;
            }

            // Calculate the VPNs for the two-level page table
            size_t vpn1 = (virtualPage >> pts2);
            size_t vpn2 = (virtualPage) & ((1LL << pts2) - 1);
            if (pageTable1[vpn1] == nullptr) {
                pageTable1[vpn1] = new vector<size_t>(pageTableSize2, -1);
            }

            // Check if the page is already mapped
            if (pageTable1[vpn1]->at(vpn2) != -1) {
                ++pageHits;
                continue;
            } else {
                ++pageMiss;
            }

            // Allocate a physical page and update the page table
            size_t physicalPage = manager->allocatePage();
            if (physicalPage != -1) {
                pageTable1[vpn1]->at(vpn2) = physicalPage;
                ++pagesAllocated;
            } else {
                cerr << "No free physical pages available for task " << id << "\n";
                return 0;
            }
            auto end = chrono::high_resolution_clock::now();
            executionTime += chrono::duration<double>(end - start).count();
        }
        return 1;
    }

    // Returns the size of the page table
    size_t getPageTableSize() const {
        size_t size = 0;
        for (const auto& entry : pageTable1) {
            if (entry != nullptr) {
                size += entry->size() * pteSize2;
            }
        }
        return pageTable1.size() * pteSize1 + size;
    }

    // Returns the task ID
    const string& getTaskId() const {
        return id;
    }

    // Returns the number of page hits
    size_t getPageHit() const {
        return pageHits;
    }

    // Returns the execution time
    double getExecutionTime() const {
        return executionTime;
    }

    // Returns the number of page misses
    size_t getPageMiss() const {
        return pageMiss;
    }

    // Returns the amount of memory allocated
    size_t memoryAllocated() const {
        return pagesAllocated * pageSize;
    }

    // Returns the number of invalid virtual pages
    size_t getInvalidVirtualPages() const {
        return invalidVirtualPages;
    }
};

#endif
