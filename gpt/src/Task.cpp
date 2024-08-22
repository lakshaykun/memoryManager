// Task.cpp
#include "../include/Task.h"
#include "../include/Config.hpp"
#include <iostream>
#include <cmath> // For floor

Task::Task(MemoryManager& manager, const std::string& id)
    : memManager(manager), taskId(id) {}

void Task::requestMemory(int logicalAddress, size_t size) {
    int pagesRequired = static_cast<int>(std::ceil(static_cast<double>(size) / pageSize));
    for (int i = 0; i < pagesRequired; ++i) {
        int virtualPage = (logicalAddress / pageSize) + i;
        if (pageTable.find(virtualPage) != pageTable.end()) {
            std::cout << "Page table hit for virtual page " << virtualPage << " in task " << taskId << "\n";
            continue;
        }
        int physicalPage = memManager.allocatePage();
        if (physicalPage != -1) {
            pageTable[virtualPage] = physicalPage;
            std::cout << "Allocated physical page " << physicalPage << " for virtual page " << virtualPage << " in task " << taskId << "\n";
        } else {
            std::cerr << "No free physical pages available for task " << taskId << "\n";
            break;
        }
    }
}

void Task::deallocateMemory() {
    for (auto& entry : pageTable) {
        memManager.deallocatePage(entry.second);
    }
    pageTable.clear();
}

size_t Task::getPageTableSize() const {
    return pageTable.size() * sizeof(int) * 2; // Size of map entries
}

std::string Task::getTaskId() const {
    return taskId;
}
