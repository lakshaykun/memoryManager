#ifndef TASK_MANAGER
#define TASK_MANAGER

#include <stdio.h>
#include <string>
#include <map>
#include "config.hpp"
#include "memoryManager.hpp"
#include "task.hpp"
using namespace std;


typedef vector<MemoryManager> Managers;

class Task {
    private:
        string id;
        TaskMap* taskMap;
        TaskSingle* taskSingle;
        TaskMulti* taskMulti;

    public:
        Task(string id, Managers* const &managers) {
            this->id = id;
            this->taskMap = new TaskMap(id, &managers->at(0));
            this->taskSingle = new TaskSingle(id, &managers->at(1));
            this->taskMulti = new TaskMulti(id, &managers->at(2));
        }

        void requestMemory(long long logicalAddress, size_t size) {
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
        Managers managers = Managers(3, MemoryManager(physicalMemorySize, virtualMemorySize, pageSize));
        vector<long long> pageHits = vector<long long>(3,0);
        vector<double> executionTime = vector<double>(3,0);

    public:
        void addTask(string id, long long logicalAddress, size_t size){
            if (tasks.find(id) == tasks.end()){
                tasks[id] = new Task(id, &managers);
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

        void displayPageHits(){
            printf("Page Hits:\n");
            printf(" Map Implementation %lu\n", pageHits[0]);
            printf(" Single Level Implementation %lu\n", pageHits[1]);
            printf(" Multi Level Implementation %lu\n", pageHits[2]);
        }

        void displayExecutionTime(){
            printf("Execution Time:\n");
            printf(" Map Implementation %f\n", executionTime[0]);
            printf(" Single Level Implementation %f\n", executionTime[1]);
            printf(" Multi Level Implementation %f\n", executionTime[2]);
        }

        void displayMemoryManager(){
            for (auto& manager : managers){
                manager.displayMemory();
            }
        }
};

#endif // TASK_MANAGER