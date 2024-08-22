// Task.h
#ifndef TASK_H
#define TASK_H

#include "MemoryManager.h"
#include <map>
#include <string>

class Task {
private:
    std::string taskId;
    std::map<int, int> pageTable; // Map: Virtual page number -> Physical page number
    MemoryManager& memManager;

public:
    Task(MemoryManager& manager, const std::string& id);
    void requestMemory(int logicalAddress, size_t size);
    void deallocateMemory();
    size_t getPageTableSize() const;
    std::string getTaskId() const;
};

#endif // TASK_H