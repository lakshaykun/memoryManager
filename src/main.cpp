#include <iostream>
#include "../include/memoryManager.hpp"
#include "../include/taskManager.hpp"
#include "../include/task.hpp"
#include "../include/config.hpp"

using namespace std;

int main() {
    // Create a MemoryManager object with example values
    MemoryManager manager(virtualMemorySize, physicalMemorySize, pageSize);
    // Display memory information
    manager.displayMemory();
    manager.addtask("jasj",20);
    // Create a TaskManager object
    TaskManager taskManager(manager);
    // Add a task to the TaskManager
    taskManager.addTask("task1");
    // Display tasks
    taskManager.displayTasks();
    // Remove a task from the TaskManager
    taskManager.removeTask("task1");
    // Display tasks
    taskManager.displayTasks();
    return 0;
}