#include<bits/stdc++.h>

#include "../include/taskManager.hpp"
#include "../include/task.hpp"
#include "../include/config.hpp"

using namespace std;
TaskManager taskManager(2);
void Trace_file_task(const string& filename){
    ifstream file(filename);
    if(!file.is_open()){
        cout<<"Error While Loading File"<<endl;
        return;
    }
    string line;
    while(getline(file,line)){
        if(line.empty()|| line.find(':')==string::npos){
            continue;
        }
        string taskid;
        string address;
        string size;

        istringstream iss(line);
        getline(iss,taskid,':');
        getline(iss,address,':');
        getline(iss,size);
       
        address=address.substr(2);
        size_t address_hex=stoull(address,nullptr,16);
        size_t size_dec;
        if (size.find("KB") != string::npos) {
        size_dec = stoull(size.substr(0, size.find("KB"))) * 1024;
        }
     else if (size.find("MB") != string::npos) {
        size_dec = stoull(size.substr(0, size.find("MB"))) * 1024 * 1024;
     }
       
        taskManager.addTask(taskid, address_hex, size_dec);
    }
}

void writeMetricsToCSV(const TaskManager& taskManager, const std::string& filename) {
    std::ofstream csvFile(filename);

    // Write CSV header
    csvFile << "Task ID,Page Hits,Page Misses,Execution Time,Page Table Size,Memory Allocated\n";

    // Fetch the metrics (already sorted by task ID)
    std::map<std::string, std::vector<double>> metrics = taskManager.tasksMetrics();

    // Write each task's metrics to the CSV file
    for (const auto& entry : metrics) {
        csvFile << entry.first << ","; // Task ID

        // Write metrics, comma-separated
        for (size_t i = 0; i < entry.second.size(); ++i) {
            csvFile << entry.second[i];
            if (i < entry.second.size() - 1) {
                csvFile << ",";
            }
        }
        csvFile << "\n";
    }

    csvFile.close();
    if (csvFile.fail()) {
        std::cerr << "Failed to write to file: " << filename << "\n";
    }
}

int main(){
    string file;
    cout<<"ENTER FILE PATH"<<endl;
    cin>>file;
    Trace_file_task(file);
    
    // Create a TaskManager object
    
    // Add a task to the TaskManager
    taskManager.displayMemoryManager();
    //csv file
    writeMetricsToCSV(taskManager,"tasks_metrics.csv");

    return 0;
}