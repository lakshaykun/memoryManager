#ifndef TASK_H
#define TASK_H
#include <stdio.h>
#include <string>
#include <map>
using namespace std;

class Task{
    private:
        string id;
        map<int, int> pageTable;

    public:
        Task(string id){
            this->id = id;
        }
    
        void setPageTable(int page, int frame){
            pageTable[page] = frame;
        }

        void requestMemory(int logicalAddress, size_t size){
            // Check if the page is in the page table
            int pageReq = size/pageSize;
            
        }

};

#endif