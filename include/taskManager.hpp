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
        size_t pageSize;
        size_t virtualMemorySize;
        size_t physicalMemorySize;
        MemoryManager manager;

    public:

        TaskManager(MemoryManager &manager) : manager(manager) {}

        void addTask(string id){
            if (tasks.find(id) == tasks.end()){
                std::cerr << "Task not found\n";
                return;
            }
            Task* task = new Task(id, manager);
            tasks[id] = task;
        }

        void removeTask(string id){
            if (tasks.find(id) == tasks.end()){
                std::cerr << "Task not found\n";
                return;
            }
            tasks[id]->deallocateMemory();
        }

        void displayTasks(){
            for (auto& task : tasks){
                printf("Task ID: %s\n", task.first.c_str());
                printf("Page Table Size: %lu bytes\n", task.second->getPageTableSize());
            }
        }
};

#endif