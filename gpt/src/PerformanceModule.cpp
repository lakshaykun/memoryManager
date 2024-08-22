// PerformanceModule.cpp
#include "../include/PerformanceModule.h"
#include <iostream>
#include "../include/Config.h"

void PerformanceModule::computeMemoryUsage(const MemoryManager& memManager, const std::vector<Task>& tasks) {
    size_t totalMemoryUsed = 0;
    for (const auto& task : tasks) {
        totalMemoryUsed += task.getPageTableSize();
    }

    size_t freeMemory = memManager.getFreePages() * PAGE_SIZE;
    std::cout << "Total Memory Used: " << totalMemoryUsed / 1024 << " KB\n";
    std::cout << "Free Memory: " << freeMemory / 1024 << " KB\n";
}
