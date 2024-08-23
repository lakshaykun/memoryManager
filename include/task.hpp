#ifndef TASK
#define TASK

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../include/config.hpp"
#include "memoryManager.hpp"

using namespace std;

typedef vector<vector<int>*> PageTable;

// TaskMap class uses a map-based page table to manage virtual to physical page mappings
class TaskMap {
private:
    string id;  // Unique identifier for the task
    map<int, int> pageTable;  // Page table mapping virtual pages to physical pages
    MemoryManager* manager;  // Pointer to the memory manager to allocate/deallocate pages

public:
    // Constructor initializes task ID and memory manager pointer
    TaskMap(string id, MemoryManager* &manager) : id(id), manager(manager) {}

    // Requests memory by allocating required pages and returns the number of page hits
    int requestMemory(int logicalAddress, size_t size) {
        int pageReq = (size + pageSize - 1) / pageSize;  // Calculate required pages, rounding up
        int pageHit = 0;  // Counter for page hits
        for (int i = 0; i < pageReq; i++) {
            // Calculate the virtual page number based on the logical address and page size
            int virtualPage = (logicalAddress + i * pageSize) / pageSize;
            // Check if the page is already in the page table (page hit)
            if (pageTable.find(virtualPage) != pageTable.end()) {
                pageHit++;
                continue;  // Skip allocation if page hit
            }

            // Allocate a new physical page if not present
            int physicalPage = manager->allocatePage();

            // Check if a free physical page is available
            if (physicalPage != -1) {
                pageTable[virtualPage] = physicalPage;  // Map virtual to physical page
            } else {
                cerr << "No free physical pages available for task\n";
                break;  // Exit if no pages are available
            }
        }
        return pageHit;  // Return the number of page hits
    }

    // Returns the size of the page table in bytes
    size_t getPageTableSize() const {
        return pageTable.size() * (sizeof(int) + sizeof(int));  // Calculate size of map entries
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
};

// TaskSingle class uses a single-level page table implemented as a vector
class TaskSingle {
private:
    string id;  // Unique identifier for the task
    vector<int> pageTable;  // Page table as a vector where each index is a virtual page number
    MemoryManager* manager;  // Pointer to the memory manager to allocate/deallocate pages

<<<<<<< HEAD
public:
    // Constructor initializes task ID, memory manager pointer, and page table size
    TaskSingle(string id, MemoryManager* &manager)
        : id(id), manager(manager), pageTable(virtualPages, -1) {}
=======
    public:
        TaskSingle(string id, MemoryManager* &manager){
            this->id = id;
            this->manager = manager;
        }
    
        void requestMemory(int logicalAddress, size_t size){
            // Check if the page is in the page table
            int pageReq = size/pageSize;
            for (int i = 0; i < pageReq; i++){
                int virtualPage = (logicalAddress + i) / pageSize;
                // if (pageTable[virtualPage] != -1){
                //     printf("Page hit\n");
                //     continue;
                // }
>>>>>>> refs/remotes/origin/main

    // Requests memory by allocating required pages
    void requestMemory(int logicalAddress, size_t size) {
        int pageReq = (size + pageSize - 1) / pageSize;  // Calculate required pages, rounding up
        for (int i = 0; i < pageReq; i++) {
            // Calculate the virtual page number based on the logical address and page size
            int virtualPage = (logicalAddress + i * pageSize) / pageSize;
            // Check if the page is already in the page table (page hit)
            if (pageTable[virtualPage] != -1) {
                printf("Page hit\n");
                continue;  // Skip allocation if page hit
            }

<<<<<<< HEAD
            // Allocate a new physical page if not present
            int physicalPage = manager->allocatePage();

            // Check if a free physical page is available
            if (physicalPage != -1) {
                pageTable[virtualPage] = physicalPage;  // Map virtual to physical page
            } else {
                cerr << "No free physical pages available for task\n";
                break;  // Exit if no pages are available
=======
                // if(physicalPage != -1) {
                //     pageTable[virtualPage] = physicalPage;
                // } else {
                //     std::cerr << "No free physical pages available for task \n";
                //     break;
                // }
>>>>>>> refs/remotes/origin/main
            }
        }
    }

    // Returns the size of the page table in bytes
    size_t getPageTableSize() const {
        return pageTable.size() * sizeof(int);  // Calculate size of vector entries
    }

    // Returns the unique identifier of the task
    string getTaskId() const {
        return id;
    }

    // Deallocates all pages used by this task and clears the page table
    void deallocateMemory() {
        for (int entry : pageTable) {
            if (entry != -1) {
                manager->deallocatePage(entry);  // Deallocate each physical page
            }
        }
        pageTable.clear();  // Clear the page table
    }
};

// TaskMulti class uses a multi-level page table implemented with vectors of vectors
class TaskMulti {
private:
    string id;  // Unique identifier for the task
    vector<int> pageTable = vector<int>(pageTableSize2, -1);  // Second-level page table
    PageTable* primaryPageTable; // Pointer to the first-level page table
    MemoryManager* manager;  // Pointer to the memory manager to allocate/deallocate pages
    int pageHits = 0;  // Counter for page hits

public:
    // Constructor initializes task ID, memory manager pointer, and the first-level page table size
    TaskMulti(string id, MemoryManager* &manager, PageTable* &primaryPageTable)
        : id(id), manager(manager), primaryPageTable(primaryPageTable) {}

    // Requests memory by allocating required pages using a two-level page table structure
    void requestMemory(int logicalAddress, size_t size) {
        int pageReq = (size + pageSize - 1) / pageSize;  // Calculate required pages, rounding up
        for (int i = 0; i < pageReq; i++) {
            // Calculate the first and second-level page table indices
            int vpn1 = (logicalAddress + i * pageSize) / pageTableSize1;
            int vpn2 = (logicalAddress + i * pageSize) % pageTableSize1;
            // Check if the first-level page table entry is present; if not, create it
            if (primaryPageTable->at(vpn1) == nullptr) {
                primaryPageTable->at(vpn1) = &pageTable;  // Map first-level to second-level page table
            }

            // Check if the second-level page table entry is a page hit
            if (pageTable.at(vpn2) != -1) {
                pageHits++;
                continue;  // Skip allocation if page hit
            }

            // Allocate a new physical page if not present
            int physicalPage = manager->allocatePage();
            if (physicalPage != -1) {
                pageTable.at(vpn2) = physicalPage;  // Map virtual to physical page
            } else {
                cerr << "No free physical pages available for task\n";
                break;  // Exit if no pages are available
            }
        }
    }

    // Returns the physical memory allocated to this task in bytes
    size_t getPageTableSize() const {
        size_t size = 0;
        for (auto& pt : pageTable) {
            if (pt != -1) {
                size += 1;  // Calculate the size of each level in the table
            }
        }
        return size*pageSize; // Return the total size of the page table
    }

    // Returns the unique identifier of the task
    string getTaskId() const {
        return id;
    }

    // Deallocates all pages used by this task and clears the multi-level page table
    void deallocateMemory() {
        for (int entry : pageTable) {
            if (entry != -1) {
                manager->deallocatePage(entry);  // Deallocate each physical page
            }
        }
        pageTable.clear();  // Clear the page table
    }

    int getPageHits() const {
        return pageHits;
    }
};

#endif
