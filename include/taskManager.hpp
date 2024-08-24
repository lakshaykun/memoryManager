#ifndef TASK_MANAGER
#define TASK_MANAGER

#include <stdio.h>
#include <string>
#include <map>
#include "../include/config.hpp"
#include "memoryManager.hpp"
#include "task.hpp"
using namespace std;


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
            taskMulti->requestMemory(logicalAddress, size);
        }

        void deallocateMemory() {
            taskMap->deallocateMemory();
            taskSingle->deallocateMemory();
            taskMulti->deallocateMemory();
        }

        vector<size_t> getPageTableSize() {
            return {taskMap->getPageTableSize(), taskSingle->getPageTableSize(), taskMulti->getPageTableSize()};
        }
};

class TaskManager {
    private:
        map<string, Task*> tasks;
        
        MemoryManager* manager = new MemoryManager(physicalMemorySize, virtualMemorySize, pageSize);
        vector<int> pageHits = vector<int>(3,0);
        vector<double> executionTime = vector<double>(3,0);

    public:
        void addTask(string id, int logicalAddress, size_t size){
            if (tasks.find(id) == tasks.end()){
                tasks[id] = new Task(id, manager);
            }
            tasks[id]->requestMemory(logicalAddress, size);
        }

        void removeTask(string id){
            if (tasks.find(id) == tasks.end()){
                std::cerr << "Task not found\n";
                return;
            }
            tasks[id]->deallocateMemory();
            tasks.erase(id);
            
        }

        void displayTasks(){
            for (auto& task : tasks){
                printf("Task ID: %s\n", task.first.c_str());
                vector<size_t> pageTableSize = task.second->getPageTableSize();
                printf("Page Table Size:\n");
                printf(" Map Implementation %lu\n", pageTableSize[0]);
                printf(" Single Level Implementation %lu\n", pageTableSize[1]);
                printf(" Multi Level Implementation %lu\n", pageTableSize[2]);
            }
        }
};

#endif // TASK_MANAGER