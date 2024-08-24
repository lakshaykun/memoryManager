#include <iostream>
#include "../include/memoryManager.hpp"
#include "../include/taskManager.hpp"
#include "../include/task.hpp"
#include "../include/config.hpp"

using namespace std;

int main() {
    // Create a TaskManager object
    TaskManager taskManager;
    // Add a task to the TaskManager
    taskManager.addTask("T1", 0704004000, 16);
    // Display tasks
    // taskManager.displayTasks();
    // Remove a task from the TaskManager
    taskManager.removeTask("T1");
    // Display tasks
    taskManager.displayTasks();

    taskManager.addTask("T2", 0704004000, 12);
    taskManager.addTask("T3", 0704004000, 8);
    taskManager.displayTasks();
    // taskManager.displayMemoryManager();
    return 0;
}