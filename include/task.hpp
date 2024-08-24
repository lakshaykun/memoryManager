#ifndef TASK
#define TASK

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <ctime>
#include "config.hpp"
#include "memoryManager.hpp"

using namespace std;

typedef vector<vector<long long>*> PageTable;

// TaskMap class uses a map-based page table to manage virtual to physical page mappings
class TaskMap {
private:
    string id;  // Unique identifier for the task
    map<long long, long long> pageTable;  // Page table mapping virtual pages to physical pages
    MemoryManager* manager;  // Polong longer to the memory manager to allocate/deallocate pages
    long long pageHit = 0;  // Counter for page hits
    long long pageMiss = 0;
    double executionTime = 0;

public:
    // Constructor initializes task ID and memory manager polong longer
    TaskMap(string id, MemoryManager* const &manager) {
        this->id = id;
        this->manager = manager;
        for (long long i=0; i<virtualPages; i++){
            pageTable[i] = -1ll;
        }
    }

    // Requests memory by allocating required pages and returns the number of page hits
    void requestMemory(long long logicalAddress, size_t size) {
        double stime = clock();
        long long pageReq = (size + pageSize - 1) / pageSize;  // Calculate required pages, rounding up
        for (long long i = 0; i < pageReq; i++) {
            // Calculate the virtual page number based on the logical address and page size
            long long virtualPage = (logicalAddress + i * pageSize) / pageSize;
            // Check if the page is already in the page table (page hit)
            if (pageTable[virtualPage] != -1) {
                pageHit++;
                continue;  // Skip allocation if page hit
            }

            // Allocate a new physical page if not present
            long long physicalPage = manager->allocatePage();

            // Check if a free physical page is available
            if (physicalPage != -1) {
                pageTable[virtualPage] = physicalPage;  // Map virtual to physical page
            } else {
                cerr << "No free physical pages available for task\n";
                break;  // Exit if no pages are available
            }
        }
        double etime = clock();
        executionTime += (etime - stime) / CLOCKS_PER_SEC;
    }

    // Returns the size of the page table in bytes
    size_t getPageTableSize() const {
        return pageTable.size() * (sizeof(long long) + sizeof(long long));  // Calculate size of map entries
    }

    // Returns the unique identifier of the task
    string getTaskId() const {
        return id;
    }

    // Deallocates all pages used by this task and clears the page table
    void deallocateMemory() {
        for (const auto& entry : pageTable) {
            manager->deallocatePage(entry.second);  // Deallocate each physical page
        }
        pageTable.clear();  // Clear the page table
    }

    long long getPageHit() const {
        return pageHit;
    }

    double getExecutionTime() const {
        return executionTime;
    }
};

// TaskSingle class uses a single-level page table implemented as a vector
class TaskSingle {
private:
    string id;  // Unique identifier for the task
    vector<long long> pageTable = vector<long long>(virtualPages, -1);  // Page table as a vector where each index is a virtual page number
    MemoryManager* manager;  // Polong longer to the memory manager to allocate/deallocate pages
    long long pageHit = 0;  // Counter for page hits
    long long pageMiss = 0;
    double executionTime = 0;

public:
    // Constructor initializes task ID, memory manager polong longer, and page table size
    TaskSingle(string id, MemoryManager* const &manager)
        : id(id), manager(manager){}

    // Requests memory by allocating required pages
    void requestMemory(long long logicalAddress, size_t size) {
        double stime = clock();
        long long pageReq = (size + pageSize - 1) / pageSize;  // Calculate required pages, rounding up
        for (long long i = 0; i < pageReq; i++) {
            // Calculate the virtual page number based on the logical address and page size
            long long virtualPage = (logicalAddress + i * pageSize) / pageSize;
            // Check if the page is already in the page table (page hit)
            if (pageTable[virtualPage] != -1) {
                pageHit++;
                continue;  // Skip allocation if page hit
            }

            // Allocate a new physical page if not present
            long long physicalPage = manager->allocatePage();

            // Check if a free physical page is available
            if (physicalPage != -1) {
                pageTable[virtualPage] = physicalPage;  // Map virtual to physical page
            } else {
                cerr << "No free physical pages available for task\n";
                break;  // Exit if no pages are available
            }
        }
        double etime = clock();
        executionTime += (etime - stime) / CLOCKS_PER_SEC;
    }

    // Returns the size of the page table in bytes
    size_t getPageTableSize() const {
        return pageTable.size() * sizeof(long long);  // Calculate size of vector entries
    }

    // Returns the unique identifier of the task
    string getTaskId() const {
        return id;
    }

    // Deallocates all pages used by this task and clears the page table
    void deallocateMemory() {
        for (long long entry : pageTable) {
            if (entry != -1) {
                manager->deallocatePage(entry);  // Deallocate each physical page
            }
        }
        pageTable.clear();  // Clear the page table
    }

    long long getPageHit() const {
        return pageHit;
    }

    double getExecutionTime() const {
        return executionTime;
    }
};

// TaskMulti class uses a multi-level page table implemented with vectors of vectors
class TaskMulti {
private:
    string id;  // Unique identifier for the task
    PageTable pageTable1 = PageTable(pageTableSize1, nullptr); // Polong longer to the first-level page table
    MemoryManager* manager;  // Polong longer to the memory manager to allocate/deallocate pages
    long long pageHits = 0;  // Counter for page hits
    long long pageMiss = 0;
    double executionTime = 0;

public:
    // Constructor initializes task ID, memory manager polong longer, and the first-level page table size
    TaskMulti(string id, MemoryManager* const &manager)
        : id(id), manager(manager){}

    // Requests memory by allocating required pages using a two-level page table structure
    void requestMemory(long long logicalAddress, size_t size) {
        double stime = clock();
        long long pageReq = (size + pageSize - 1) / pageSize;  // Calculate required pages, rounding up
        for (long long i = 0; i < pageReq; i++) {
            // Calculate the first and second-level page table indices
                long long vpn1 = (logicalAddress >> 22) & ((1LL << 10) - 1);  // Extracting the first 10 bits
                long long vpn2 = (logicalAddress >> 12) & ((1LL << 10) - 1);  // Extracting the next 10 bits
            // Check if the first-level page table entry is present; if not, create it
            if (pageTable1.at(vpn1) == nullptr) {
                pageTable1.at(vpn1) = new vector<long long>(pageTableSize2, -1);  // Map first-level to second-level page table
            }

            // Check if the second-level page table entry is a page hit
            if (pageTable1.at(vpn1)->at(vpn2) != -1) {
                pageHits++;
                continue;  // Skip allocation if page hit
            }

            // Allocate a new physical page if not present
            long long physicalPage = manager->allocatePage();
            if (physicalPage != -1) {
                pageTable1.at(vpn1)->at(vpn2) = physicalPage;  // Map virtual to physical page
            } else {
                cerr << "No free physical pages available for task\n";
                break;  // Exit if no pages are available
            }
        }
        double etime = clock();
        executionTime += (etime - stime) / CLOCKS_PER_SEC;
    }

    // Returns the physical memory allocated to this task in bytes
    size_t getPageTableSize() const {
        size_t size = 0;
        for (const auto& entry : pageTable1) {
            if (entry != nullptr) {
                size += entry->size() * sizeof(long long);  // Calculate size of second-level page table
            }
        }
        return pageTable1.size() * sizeof(vector<long long>*) + size;  // Calculate size of first-level page table
    }

    // Returns the unique identifier of the task
    string getTaskId() const {
        return id;
    }

    // Deallocates all pages used by this task and clears the multi-level page table
    void deallocateMemory() {
        for (auto& entry : pageTable1) {
            if (entry != nullptr) {
                for (long long page : *entry) {
                    if (page != -1) {
                        manager->deallocatePage(page);  // Deallocate each physical page
                    }
                }
                delete entry;  // Clear the second-level page table
            }
        }
        pageTable1.clear();  // Clear the first-level page table
    }

    long long getPageHit() const {
        return pageHits;
    }

    double getExecutionTime() const {
        return executionTime;
    }
};

#endif
