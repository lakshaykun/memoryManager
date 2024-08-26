#include <iostream>
#include "../include/taskManager.hpp"
#include "input_output.hpp"
#include "../include/task.hpp"
#include "../include/config.hpp"

using namespace std;

int main(){
    string file;
    cout<<"ENTER FILE PATH"<<endl;
    cin>>file;
    Trace_file_task(file);
    
    // Create a TaskManager object
    
    // Add a task to the TaskManager
    taskManager.displayMemoryManager();
    writeMetricsToCSV(taskManager, "tasks_metrics.csv");
    
    return 0;
}