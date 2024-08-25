// #ifndef TASK_MANAGER
// #define TASK_MANAGER

// #include <stdio.h>
// #include <string>
// #include <map>
// #include "config.hpp"
// #include "memoryManager.hpp"
// #include "task.hpp"
// using namespace std;


// typedef vector<MemoryManager> Managers;

// class Task {
//     private:
//         string id;
//         TaskMap* taskMap;
//         TaskSingle* taskSingle;
//         vector<long long> pageHits = vector<long long>(3, 0);
//         vector<double> executionTime = vector<double>(3, 0);
//         TaskMulti* taskMulti;

//     public:
//         Task(string id, Managers* const &managers) {
//             this->id = id;
//             this->taskMap = new TaskMap(id, &managers->at(0));
//             this->taskSingle = new TaskSingle(id, &managers->at(1));
//             this->taskMulti = new TaskMulti(id, &managers->at(2));
//         }

//         void requestMemory(long long logicalAddress, size_t size) {
//             taskMap->requestMemory(logicalAddress, size);
//             taskSingle->requestMemory(logicalAddress, size);
//             taskMulti->requestMemory(logicalAddress, size);
//         }

//         void deallocateMemory() {
//             taskMap->deallocateMemory();
//             taskSingle->deallocateMemory();
//             taskMulti->deallocateMemory();
//         }

//         vector<size_t> getPageTableSize() {
//             return {taskMap->getPageTableSize(), taskSingle->getPageTableSize(), taskMulti->getPageTableSize()};
//         }

//         vector<long long> getPageHits(){
//             pageHits[0] = taskMap->getPageHit();
//             pageHits[1] = taskSingle->getPageHit();
//             pageHits[2] = taskMulti->getPageHit();
//             return pageHits;
//         }

//         vector<double> getExecutionTime(){
//             executionTime[0] = taskMap->getExecutionTime();
//             executionTime[1] = taskSingle->getExecutionTime();
//             executionTime[2] = taskMulti->getExecutionTime();
//             return executionTime;
//         }
// };

// class TaskManager {
//     private:
//         map<string, Task*> tasks;
//         Managers managers = Managers(3, MemoryManager(physicalMemorySize, virtualMemorySize, pageSize));
//         vector<long long> pageHits = vector<long long>(3,0);
//         vector<double> executionTime = vector<double>(3,0);

//     public:
//         void addTask(string id, long long logicalAddress, size_t size){
//             if (tasks.find(id) == tasks.end()){
//                 tasks[id] = new Task(id, &managers);
//             }
//             tasks[id]->requestMemory(logicalAddress, size);
//         }

//         void removeTask(string id){
//             if (tasks.find(id) == tasks.end()){
//                 std::cerr << "Task not found\n";
//                 return;
//             }
//             tasks[id]->deallocateMemory();
//             tasks.erase(id);
            
//         }

//         void displayTasks(){
//             for (auto& task : tasks){
//                 printf("Task ID: %s\n", task.first.c_str());
//                 vector<size_t> pageTableSize = task.second->getPageTableSize();
//                 printf("Page Table Size:\n");
//                 printf(" Map Implementation %lu\n", pageTableSize[0]);
//                 printf(" Single Level Implementation %lu\n", pageTableSize[1]);
//                 printf(" Multi Level Implementation %lu\n", pageTableSize[2]);
//             }
//         }

//         void getPageHits(){
//             vector<long long> tempHits = vector<long long>(3,0);
//             for (auto& task : tasks){
//                 vector<long long> hits = task.second->getPageHits();
//                 for (int i = 0; i < 3; i++){
//                     tempHits[i] += hits[i];
//                 }
//             }
//             pageHits = tempHits;
//         }

//         void getExecutionTime(){
//             vector<double> tempTime = vector<double>(3,0);
//             for (auto& task : tasks){
//                 vector<double> times = task.second->getExecutionTime();
//                 for (int i = 0; i < 3; i++){
//                     tempTime[i] += times[i];
//                 }
//             }
//             executionTime = tempTime;
//         }

//         void displayPageHits(){
//             getPageHits();
//             cout << "Page Hits:\n";
//             cout << " Map Implementation " << pageHits[0] << endl;
//             cout << " Single Level Implementation " << pageHits[1] << endl;
//             cout << " Multi Level Implementation " << pageHits[2] << endl;
//         }

//         void displayExecutionTime(){
//             getExecutionTime();
//             cout << "Execution Time:\n";
//             cout << " Map Implementation " << executionTime[0] << endl;
//             cout << " Single Level Implementation " << executionTime[1] << endl;
//             cout << " Multi Level Implementation " << executionTime[2] << endl;
//         }

//         void displayMemoryManager(){
//             for (auto& manager : managers){
//                 manager.displayMemory();
//             }
//         }
// };

// #endif // TASK_MANAGER

#ifndef TASK_MANAGER
#define TASK_MANAGER

#include <iostream>
#include <string>
#include <map>
#include <vector>
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
    vector<long long> pageHits;
    vector<double> executionTime;

public:
    Task(const string& id, Managers* const &managers)
        : id(id), 
          taskMap(new TaskMap(id, &managers->at(0))),
          taskSingle(new TaskSingle(id, &managers->at(1))),
          taskMulti(new TaskMulti(id, &managers->at(2))),
          pageHits(3, 0),
          executionTime(3, 0.0) {}

    ~Task() {
        delete taskMap;
        delete taskSingle;
        delete taskMulti;
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

    vector<size_t> getPageTableSize() const {
        return {taskMap->getPageTableSize(), taskSingle->getPageTableSize(), taskMulti->getPageTableSize()};
    }

    vector<long long> getPageHits() {
        pageHits[0] = taskMap->getPageHit();
        pageHits[1] = taskSingle->getPageHit();
        pageHits[2] = taskMulti->getPageHit();
        return pageHits;
    }

    vector<double> getExecutionTime() {
        executionTime[0] = taskMap->getExecutionTime();
        executionTime[1] = taskSingle->getExecutionTime();
        executionTime[2] = taskMulti->getExecutionTime();
        return executionTime;
    }
};

class TaskManager {
private:
    map<string, Task*> tasks;
    Managers managers;
    vector<long long> pageHits;
    vector<double> executionTime;

public:
    TaskManager() 
        : managers(3, MemoryManager(physicalMemorySize, virtualMemorySize, pageSize)),
          pageHits(3, 0), 
          executionTime(3, 0.0) {}

    ~TaskManager() {
        for (auto& task : tasks) {
            delete task.second;
        }
    }

    void addTask(const string& id, long long logicalAddress, size_t size) {
        if (tasks.find(id) == tasks.end()) {
            tasks[id] = new Task(id, &managers);
        }
        tasks[id]->requestMemory(logicalAddress, size);
    }

    void removeTask(const string& id) {
        auto it = tasks.find(id);
        if (it == tasks.end()) {
            cerr << "Task not found\n";
            return;
        }
        it->second->deallocateMemory();
        delete it->second;
        tasks.erase(it);
    }

    void displayTasks() const {
        for (const auto& task : tasks) {
            cout << "Task ID: " << task.first << endl;
            vector<size_t> pageTableSize = task.second->getPageTableSize();
            cout << "Page Table Size:\n";
            cout << " Map Implementation: " << pageTableSize[0] << endl;
            cout << " Single Level Implementation: " << pageTableSize[1] << endl;
            cout << " Multi-Level Implementation: " << pageTableSize[2] << endl;
        }
    }

    void calculatePageHits() {
        fill(pageHits.begin(), pageHits.end(), 0);
        for (const auto& task : tasks) {
            vector<long long> hits = task.second->getPageHits();
            for (int i = 0; i < 3; ++i) {
                pageHits[i] += hits[i];
            }
        }
    }

    void calculateExecutionTime() {
        fill(executionTime.begin(), executionTime.end(), 0.0);
        for (const auto& task : tasks) {
            vector<double> times = task.second->getExecutionTime();
            for (int i = 0; i < 3; ++i) {
                executionTime[i] += times[i];
            }
        }
    }

    void displayPageHits() {
        calculatePageHits();
        cout << "Page Hits:\n";
        cout << " Map Implementation: " << pageHits[0] << endl;
        cout << " Single Level Implementation: " << pageHits[1] << endl;
        cout << " Multi-Level Implementation: " << pageHits[2] << endl;
    }

    void displayExecutionTime() {
        calculateExecutionTime();
        cout << "Execution Time:\n";
        cout << " Map Implementation: " << executionTime[0] << endl;
        cout << " Single Level Implementation: " << executionTime[1] << endl;
        cout << " Multi-Level Implementation: " << executionTime[2] << endl;
    }

    void displayMemoryManager() const {
        for (const auto& manager : managers) {
            manager.displayMemory();
        }
    }
};

#endif // TASK_MANAGER
