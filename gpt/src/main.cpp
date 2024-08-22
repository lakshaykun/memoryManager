// main.cpp
#include "../include/MemoryManager.h"
#include "../include/Task.h"
#include "../include/PerformanceModule.h"
#include "../include/IO_Module.h"
#include "../include/TestModule.h"
#include "../include/Config.h"
#include <vector>

int main() {
    // Initialize memory manager with physical memory size in pages
    MemoryManager memoryManager(PHYSICAL_MEMORY_SIZE / PAGE_SIZE);

    // Create tasks
    std::vector<Task> tasks;
    tasks.emplace_back(memoryManager, "T1");
    tasks.emplace_back(memoryManager, "T2");

    // Generate and process trace file
    generateTrace(2);
    processTrace("trace.txt", memoryManager, tasks);

    // Compute and display memory usage
    PerformanceModule::computeMemoryUsage(memoryManager, tasks);

    return 0;
}
