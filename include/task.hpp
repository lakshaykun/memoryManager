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

typedef vector<vector<int>*> PageTable;

// TaskMap class uses a map-based page table to manage virtual to physical page mappings
class TaskMap {
private:
    string id;  // Unique identifier for the task
    map<size_t, int> pageTable;  // Page table mapping virtual pages to physical pages
    MemoryManager* manager;  // Pointer to the memory manager to allocate/deallocate pages
    size_t pageHit = 0;  // Counter for page hits
    size_t pageMiss = 0;  // Counter for page misses
    double executionTime = 0.0;  // Execution time tracking

public:
    // Constructor initializes task ID and memory manager pointer
    TaskMap(const string& id, MemoryManager* const &manager)
        : id(id), manager(manager), pageHit(0), pageMiss(0), executionTime(0) {
        // Initialize all virtual pages to -1 (indicating no mapping)
        for (size_t i = 0; i < virtualPages; ++i) {
            pageTable[i] = -1;
        }
    }

    // Requests memory by allocating required pages and returns the number of page hits
    void requestMemory(size_t logicalAddress, size_t size) {
        auto start = chrono::high_resolution_clock::now();
        size_t pageReq = (int) ceil((double) size / pageSize);  // Calculate required pages, rounding up
        for (size_t i = 0; i < pageReq; ++i) {
            size_t virtualPage = (logicalAddress >> p) + i;  // Calculate the virtual page number
            if (pageTable[virtualPage] != -1) {
                ++pageHit;
                continue;
            } else {
                ++pageMiss;
            }

            int physicalPage = manager->allocatePage();
            if (physicalPage != -1) {
                pageTable[virtualPage] = physicalPage;
            } else {
                cerr << "No free physical pages available for task " << id << "\n";
                break;
            }
        }
        auto end = chrono::high_resolution_clock::now();
        executionTime += chrono::duration<double>(end - start).count();
    }

    size_t getPageTableSize() const {
        return pageTable.size() * (pteSize1 + pteSize2);
    }

    const string& getTaskId() const {
        return id;
    }

    void deallocateMemory() {
        for (const auto& entry : pageTable) {
            if (entry.second != -1) {
                manager->deallocatePage(entry.second);
            }
        }
        pageTable.clear();
    }

    size_t getPageHit() const {
        return pageHit;
    }

    double getExecutionTime() const {
        return executionTime;
    }

    size_t getPageMiss() const {
        return pageMiss;
    }
};

// TaskSingle class uses a single-level page table implemented as a vector
class TaskSingle {
private:
    string id;  // Unique identifier for the task
    vector<int> pageTable;  // Page table as a vector where each index is a virtual page number
    MemoryManager* manager;  // Pointer to the memory manager to allocate/deallocate pages
    size_t pageHit = 0;  // Counter for page hits
    size_t pageMiss = 0;  // Counter for page misses
    double executionTime = 0.0;  // Execution time tracking

public:
    // Constructor initializes task ID, memory manager pointer, and page table size
    TaskSingle(const string& id, MemoryManager* const &manager)
        : id(id), manager(manager), pageTable(virtualPages, -1), pageHit(0), pageMiss(0), executionTime(0) {}

    // Requests memory by allocating required pages
    void requestMemory(size_t logicalAddress, size_t size) {
        auto start = chrono::high_resolution_clock::now();
        size_t pageReq = (int) ceil((double) size / pageSize);
        for (size_t i = 0; i < pageReq; ++i) {
            size_t virtualPage = (logicalAddress >> p) + i; // Calculate the virtual page number
            if (pageTable[virtualPage] != -1) {
                ++pageHit;
                continue;
            } else {
                ++pageMiss;
            }

            int physicalPage = manager->allocatePage();
            if (physicalPage != -1) {
                pageTable[virtualPage] = physicalPage;
            } else {
                cerr << "No free physical pages available for task " << id << "\n";
                break;
            }
        }
        auto end = chrono::high_resolution_clock::now();
        executionTime += chrono::duration<double>(end - start).count();
    }

    size_t getPageTableSize() const {
        return pageTable.size() * pteSize2;
    }

    const string& getTaskId() const {
        return id;
    }

    void deallocateMemory() {
        for (int entry : pageTable) {
            if (entry != -1) {
                manager->deallocatePage(entry);
            }
        }
        pageTable.clear();
    }

    size_t getPageHit() const {
        return pageHit;
    }

    double getExecutionTime() const {
        return executionTime;
    }

    size_t getPageMiss() const {
        return pageMiss;
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

public:
    // Constructor initializes task ID, memory manager pointer, and the first-level page table size
    TaskMulti(const string& id, MemoryManager* const &manager)
        : id(id), manager(manager), pageTable1(pageTableSize1, nullptr), pageHits(0), pageMiss(0), executionTime(0) {}

    // Requests memory by allocating required pages using a two-level page table structure
    void requestMemory(size_t logicalAddress, size_t size) {
        auto start = chrono::high_resolution_clock::now();
        size_t pageReq = (int) ceil((double) size / pageSize);
        for (size_t i = 0; i < pageReq; ++i) {
            size_t vpn1 = (logicalAddress >> (p+pts2)) & ((1LL << pts1) - 1);
            size_t vpn2 = (logicalAddress >> p) & ((1LL << pts2) - 1);
            
            if (pageTable1[vpn1] == nullptr) {
                pageTable1[vpn1] = new vector<int>(pageTableSize2, -1);
            }

            if (pageTable1[vpn1]->at(vpn2) != -1) {
                ++pageHits;
                continue;
            } else {
                ++pageMiss;
            }

            int physicalPage = manager->allocatePage();
            if (physicalPage != -1) {
                pageTable1[vpn1]->at(vpn2) = physicalPage;
            } else {
                cerr << "No free physical pages available for task " << id << "\n";
                break;
            }
        }
        auto end = chrono::high_resolution_clock::now();
        executionTime += chrono::duration<double>(end - start).count();
    }

    size_t getPageTableSize() const {
        size_t size = 0;
        for (const auto& entry : pageTable1) {
            if (entry != nullptr) {
                size += entry->size() * pteSize2;
            }
        }
        return pageTable1.size() * pteSize1 + size;
    }

    const string& getTaskId() const {
        return id;
    }

    void deallocateMemory() {
        for (auto& entry : pageTable1) {
            if (entry != nullptr) {
                for (int page : *entry) {
                    if (page != -1) {
                        manager->deallocatePage(page);
                    }
                }
                delete entry;
            }
        }
        pageTable1.clear();
    }

    size_t getPageHit() const {
        return pageHits;
    }

    double getExecutionTime() const {
        return executionTime;
    }

    size_t getPageMiss() const {
        return pageMiss;
    }
};

#endif
