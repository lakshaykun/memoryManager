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
    int type;
    TaskMap* taskMap;
    TaskSingle* taskSingle;
    TaskMulti* taskMulti;

public:
    Task(const string& id, Managers* const &managers, const int& type)
        : id(id),
            type(type), 
          taskMap(new TaskMap(id, &managers->at(0))),
          taskSingle(new TaskSingle(id, &managers->at(1))),
          taskMulti(new TaskMulti(id, &managers->at(2))) {}

    ~Task() {
        delete taskMap;
        delete taskSingle;
        delete taskMulti;
    }

    void requestMemory(long long logicalAddress, size_t size) {
        // switch case
        if (type == 0) {
            taskMap->requestMemory(logicalAddress, size);
        } else if (type == 1) {
            taskSingle->requestMemory(logicalAddress, size);
        } else {
            taskMulti->requestMemory(logicalAddress, size);
        }
    }

    void deallocateMemory() {
        // switch case
        if (type == 0) {
            taskMap->deallocateMemory();
        } else if (type == 1) {
            taskSingle->deallocateMemory();
        } else {
            taskMulti->deallocateMemory();
        }
    }

    size_t getPageTableSize() const {
        // switch case
        if (type == 0) {
            return taskMap->getPageTableSize();
        } else if (type == 1) {
            return taskSingle->getPageTableSize();
        } else {
            return taskMulti->getPageTableSize();
        }
    }

    size_t getPageHits() {
        // switch case
        if (type == 0) {
            return taskMap->getPageHit();
        } else if (type == 1) {
            return taskSingle->getPageHit();
        } else {
            return taskMulti->getPageHit();
        }
    }

    double getExecutionTime() {
        // switch case
        if (type == 0) {
            return taskMap->getExecutionTime();
        } else if (type == 1) {
            return taskSingle->getExecutionTime();
        } else {
            return taskMulti->getExecutionTime();
        }
    }

    string getTaskId() const {
        return id;
    }

    size_t getPageMiss() {
        // switch case
        if (type == 0) {
            return taskMap->getPageMiss();
        } else if (type == 1) {
            return taskSingle->getPageMiss();
        } else {
            return taskMulti->getPageMiss();
        }
    }

    size_t getMemoryAllocated() {
        // switch case
        if (type == 0) {
            return taskMap->memoryAllocated();
        } else if (type == 1) {
            return taskSingle->memoryAllocated();
        } else {
            return taskMulti->memoryAllocated();
        }
    }

};

class TaskManager {
private:
    map<string, Task*> tasks;
    Managers managers;
    int type;
    double pageHits = 0;
    double pageMiss = 0;
    double executionTime = 0;

public:
    TaskManager(const int& type) 
        : managers(3, MemoryManager(physicalMemorySize, virtualMemorySize, pageSize)),
          type(type){}

    ~TaskManager() {
        for (auto& task : tasks) {
            delete task.second;
        }
    }

    void addTask(const string& id, long long logicalAddress, size_t size) {
        if (tasks.find(id) == tasks.end()) {
            tasks[id] = new Task(id, &managers, type);
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

    void calculatePageHits() {
        pageHits = 0;
        for (const auto& task : tasks) {
            pageHits += task.second->getPageHits();
        }
    }

    void calculateExecutionTime() {
        executionTime = 0;
        for (const auto& task : tasks) {
            executionTime += task.second->getExecutionTime();
        }
    }

    void calculatePageMiss() {
        pageMiss = 0;
        for (const auto& task : tasks) {
            pageMiss += task.second->getPageMiss();
        }
    }

    void displayMemoryManager() const {
        for (const auto& manager : managers) {
            manager.displayMemory();
        }
    }

    vector<double> metrics() {
        calculatePageHits();
        calculateExecutionTime();
        calculatePageMiss();
        return {pageHits, pageMiss, executionTime};
    }

    map<string, vector<double>> tasksMetrics() const {
        map<string, vector<double>> metrics;
        for (const auto& task : tasks) {
            vector<double> metric;
            metric.push_back(task.second->getPageHits());
            metric.push_back(task.second->getPageMiss());
            metric.push_back(task.second->getExecutionTime());
            metric.push_back(task.second->getPageTableSize());
            metric.push_back(task.second->getMemoryAllocated());
            metrics[task.first] = metric;
        }
        return metrics;
    }

        
};




#endif // TASK_MANAGER
