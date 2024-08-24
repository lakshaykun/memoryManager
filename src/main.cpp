#include <iostream>
#include "../include/taskManager.hpp"
using namespace std;

int main() {
    // Create a TaskManager object
    TaskManager taskManager;
    // Add a task to the TaskManager
    taskManager.addTask("T1", 0704004000, 1<<14);
    // Display tasks
    // taskManager.displayTasks();
    // Remove a task from the TaskManager
    // taskManager.removeTask("T1");
    // Display tasks
    taskManager.displayTasks();

    taskManager.addTask("T2", 1704004000, 1<<12);
    taskManager.addTask("T3", 3704004000, 1<<16);
    taskManager.displayTasks();
    // taskManager.displayMemoryManager();
    return 0;
}