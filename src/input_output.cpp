#include<bits/stdc++.h>

#include "../include/taskManager.hpp"
#include "../include/task.hpp"
#include "../include/config.hpp"

using namespace std;
TaskManager taskManager(1);
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
       
        bool alert = taskManager.addTask(taskid, address_hex, size_dec);
        if (!alert) {
            cerr << "Memory allocation failed for task " << taskid << "\n";
            break;
        }
    }
}
int extractNumericPart(const std::string& s) {
    std::stringstream ss(s.substr(1)); // Remove the leading character and convert the rest to an integer
    int num = 0;
    ss >> num;
    return num;
}

// Comparator function to sort based on the numeric part of the task ID
bool compareTaskIDs(const std::string& id1, const std::string& id2) {
    return extractNumericPart(id1) < extractNumericPart(id2);
}
void writeMetricsToCSV(const TaskManager& taskManager, const std::string& filename) {
    std::ofstream csvFile(filename);

    // Write CSV header
    csvFile << "Task ID,Page Hits,Page Misses,Execution Time,Page Table Size,Memory Allocated,Invalid Vpn\n";

    // Fetch the metrics (already sorted by task ID)
    std::map<std::string, std::vector<double>> metrics = taskManager.tasksMetrics();

    // Collect task IDs for custom sorting
    std::vector<std::string> taskIDs;
    for (const auto& entry : metrics) {
        taskIDs.push_back(entry.first);
    }

    // Sort task IDs using the custom comparator
    std::sort(taskIDs.begin(), taskIDs.end(), compareTaskIDs);

    // Write each task's metrics to the CSV file
    for (const auto& taskID : taskIDs) {
        const auto& entry = metrics[taskID];
        csvFile << taskID << ","; // Task ID

        // Write metrics, comma-separated
        for (size_t i = 0; i < entry.size(); ++i) {
            csvFile << entry[i];
            if (i < entry.size() - 1) {
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