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
                int page = (logicalAddress + i) / pageSize;
                if (pageTable.find(page) != pageTable.end()){
                    printf("Page hit\n");
                    continue;
                }

                int physicalPage = manager->allocatePage();
                
            }
        }
};

#endif