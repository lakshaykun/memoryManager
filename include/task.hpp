#ifndef TASK_H
#define TASK_H
#include <stdio.h>
#include <string>
#include <map>
#include "../include/config.hpp"
#include "memoryManager.hpp"
using namespace std;

class Task{
    private:
        string id;
        map<int, int> pageTable;
        MemoryManager* manager;

    public:
        Task(string id, MemoryManager* &manager){
            this->id = id;
            this->manager = manager;
        }
    
        void setPageTable(int page, int frame){
            pageTable[page] = frame;
        }

        void requestMemory(int logicalAddress, size_t size){
            // Check if the page is in the page table
            int pageReq = size/pageSize;
            for (int i = 0; i < pageReq; i++){
                int virtualPage = (logicalAddress + i) / pageSize;
                if (pageTable.find(virtualPage) != pageTable.end()){
                    printf("Page hit\n");
                    continue;
                }

                int physicalPage = manager->allocatePage();

                if(physicalPage != -1) {
                    pageTable[virtualPage] = physicalPage;
                } else {
                    std::cerr << "No free physical pages available for task \n";
                    break;
                }
            }
        }

        size_t getPageTableSize() {
            return pageTable.size() * sizeof(int) * 2; // Size of map entries
        }

        string getTaskId() {
            return id;
        }

        void deallocateMemory() {
            for (auto& entry : pageTable) {
                manager->deallocatePage(entry.second);
            }
            pageTable.clear();
        }
};

#endif