// PerformanceModule.h
#ifndef PERFORMANCE_MODULE_H
#define PERFORMANCE_MODULE_H

#include "../include/MemoryManager.h"
#include "../include/Task.h"
#include <vector>

class PerformanceModule {
public:
    static void computeMemoryUsage(const MemoryManager& memManager, const std::vector<Task>& tasks);
};

#endif // PERFORMANCE_MODULE_H
