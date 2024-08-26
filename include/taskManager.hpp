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

class Task {
private:
    string id;
    int type;
    TaskMap* taskMap;
    TaskSingle* taskSingle;
    TaskMulti* taskMulti;

public:
    Task(const string& id, MemoryManager* const &manager, const int& type)
        :   id(id),
            type(type) {
        if (type == 0) {
            taskMap = new TaskMap(id, manager);
        } else if (type == 1) {
            taskSingle = new TaskSingle(id, manager);
        } else {
            taskMulti = new TaskMulti(id, manager);
        }
    }

    ~Task() {
        delete taskMap;
        delete taskSingle;
        delete taskMulti;
    }

    bool requestMemory(long long logicalAddress, size_t size) {
        // switch case
        bool alert = false;
        if (type == 0) {
            alert = taskMap->requestMemory(logicalAddress, size);
        } else if (type == 1) {
            alert = taskSingle->requestMemory(logicalAddress, size);
        } else {
            alert = taskMulti->requestMemory(logicalAddress, size);
        }
        return alert;
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

    double getPageTableSize() const {
    double pageTableSizeInKB;

    switch (type) {
        case 0:
            pageTableSizeInKB = static_cast<double>(taskMap->getPageTableSize()) / 1024.0;
            break;
        case 1:
            pageTableSizeInKB = static_cast<double>(taskSingle->getPageTableSize()) / 1024.0;
            break;
        case 2:
            pageTableSizeInKB = static_cast<double>(taskMulti->getPageTableSize()) / 1024.0;
            break;
        default:
            pageTableSizeInKB = 0; // or handle as an error if an invalid type
            break;
    }

    return pageTableSizeInKB;
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

   double getMemoryAllocated() const {
    double memoryInKB;
    
    if (type == 0) {
        memoryInKB = static_cast<double>(taskMap->memoryAllocated()) / 1024.0;
    } else if (type == 1) {
        memoryInKB = static_cast<double>(taskSingle->memoryAllocated()) / 1024.0;
    } else {
        memoryInKB = static_cast<double>(taskMulti->memoryAllocated()) / 1024.0;
    }

    return memoryInKB;
}
    
        size_t getInvalidVirtualPages() {
            // switch case
            if (type == 0) {
                return taskMap->getInvalidVirtualPages();
            } else if (type == 1) {
                return taskSingle->getInvalidVirtualPages();
            } else {
                return taskMulti->getInvalidVirtualPages();
            }
        }
};

class TaskManager {
private:
    map<string, Task*> tasks;
    MemoryManager manager;
    int type;
    double pageHits = 0;
    double pageMiss = 0;
    double executionTime = 0;
    

public:

    TaskManager(const int& type) 
        : manager(MemoryManager(physicalMemorySize, virtualMemorySize, pageSize)),
          type(type){}

    ~TaskManager() {
        for (auto& task : tasks) {
            delete task.second;
        }
    }

    bool addTask(const string& id, long long logicalAddress, size_t size) {
        if (tasks.find(id) == tasks.end()) {
            tasks[id] = new Task(id, &manager, type);
        }
        return tasks[id]->requestMemory(logicalAddress, size);
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
        manager.displayMemory();
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
            metric.push_back(task.second->getInvalidVirtualPages());
            metrics[task.first] = metric;
        }
        return metrics;
    }

        
};




#endif // TASK_MANAGER
