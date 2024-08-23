#ifndef TASK_MANAGER
#define TASK_MANAGER

#include <stdio.h>
#include <string>
#include <map>
#include "../include/config.hpp"
#include "memoryManager.hpp"
#include "task.hpp"
using namespace std;

class TaskManager {
    private:
        map<string, Task*> tasks;
        MemoryManager* manager = new MemoryManager(physicalMemorySize, virtualMemorySize, pageSize);

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