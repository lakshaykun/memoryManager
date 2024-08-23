#ifndef TASK
#define TASK
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "../include/config.hpp"
#include "memoryManager.hpp"
using namespace std;


class TaskMap{
    private:
        string id;
        map<int, int> pageTable;
        MemoryManager* manager;

    public:
        TaskMap(string id, MemoryManager* &manager){
            this->id = id;
            this->manager = manager;
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

class TaskSingle{
    private:
        string id;
        vector<int> pageTable = vector<int>(virtualPages, -1);
        MemoryManager* manager;

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
                if (pageTable[virtualPage] != -1){
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
            return pageTable.size() * sizeof(int); // Size of map entries
        }

        string getTaskId() {
            return id;
        }

        void deallocateMemory() {
            for (auto& entry : pageTable) {
                manager->deallocatePage(entry);
            }
            pageTable.clear();
        }
};


class TaskMulti {
    private:
        string id;
        vector<vector<int>*> pageTable = vector<vector<int>*>(pageTableSize1, nullptr);
        MemoryManager* manager;

    public:
        TaskMulti(string id, MemoryManager* &manager){
            this->id = id;
            this->manager = manager;
        }
    
        void requestMemory(int logicalAddress, size_t size){
            // Check if the page is in the page table
            int pageReq = size/pageSize;
            for (int i = 0; i < pageReq; i++){
                int vpn1 = (logicalAddress + i) / pageTableSize1;
                int vpn2 = (logicalAddress + i) % pageTableSize1;
                if (pageTable[vpn1] == nullptr){
                    int physicalAddress = manager->allocatePage();
                    pageTable[vpn1] = new vector<int>(pageTableSize2, -1);
                    pageTable[vpn1]->at(vpn2) = physicalAddress;
                }
            }
        }

        size_t getPageTableSize() {
            return pageTable.size() * sizeof(int) * 2; // Size of map entries
        }

        string getTaskId() {
            return id;
        }

        // void deallocateMemory() {
        //     for (auto& entry : pageTable) {
        //         manager->deallocatePage(entry.second);
        //     }
        //     pageTable.clear();
        // }
};

class Task {
    private:
        string id;
        TaskMap* taskMap;
        TaskSingle* taskSingle;
        TaskMulti* taskMulti;

    public:
        Task(string id, MemoryManager* &manager) {
            this->id = id;
            this->taskMap = new TaskMap(id, manager);
            this->taskSingle = new TaskSingle(id, manager);
            this->taskMulti = new TaskMulti(id, manager);
        }

        void requestMemory(int logicalAddress, size_t size) {
            taskMap->requestMemory(logicalAddress, size);
            taskSingle->requestMemory(logicalAddress, size);
            // taskMulti->requestMemory(logicalAddress, size);
        }

        void deallocateMemory() {
            taskMap->deallocateMemory();
            taskSingle->deallocateMemory();
            // taskMulti->deallocateMemory();
        }

        vector<size_t> getPageTableSize() {
            return {taskMap->getPageTableSize(), taskSingle->getPageTableSize(), taskMulti->getPageTableSize()};
        }
};

#endif