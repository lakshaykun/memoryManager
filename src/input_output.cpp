#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "../include/taskManager.hpp"
#include "../include/task.hpp"
#include "../include/config.hpp"

using namespace std;

// Helper function to extract numeric part of a string (e.g., "t10" -> 10)
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

// Function to process the trace file and add tasks
void Trace_file_task(const string& filename,TaskManager &taskManager) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error While Loading File: " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty() || line.find(':') == string::npos) {
            continue;
        }
        
        string taskid, address, size;
        istringstream iss(line);
        getline(iss, taskid, ':');
        getline(iss, address, ':');
        getline(iss, size);

        address = address.substr(2); // Remove leading '0x' from hexadecimal address
        size_t address_hex = stoull(address, nullptr, 16);
        size_t size_dec;
        if (size.find("KB") != string::npos) {
            size_dec = stoull(size.substr(0, size.find("KB"))) * 1024;
        } else if (size.find("MB") != string::npos) {
            size_dec = stoull(size.substr(0, size.find("MB"))) * 1024 * 1024;
        } else {
            cerr << "Unknown size unit for task " << taskid << "\n";
            continue;
        }

        bool alert = taskManager.addTask(taskid, address_hex, size_dec);
        if (!alert) {
            cerr << "Memory allocation failed for task " << taskid << "\n";
            break;
        }
    }
}

// Function to write metrics to a CSV file
void writeMetricsToCSV( TaskManager& taskManager, const string& filename) {
    ofstream csvFile(filename);
    if (!csvFile.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    // Write CSV header
    csvFile << "Task ID,Page Hits,Page Misses,Execution Time,Page Table Size,Physical Memory Used,Invalid Pages\n";

    // Fetch the metrics
    map<string, vector<double>> metrics1 = taskManager.tasksMetrics();

    // Collect task IDs for custom sorting
    vector<string> taskIDs;
    for (const auto& entry : metrics1) {
        taskIDs.push_back(entry.first);
    }

    // Sort task IDs using the custom comparator
    sort(taskIDs.begin(), taskIDs.end(), compareTaskIDs);

    // Write each task's metrics to the CSV file
    for (const auto& taskID : taskIDs) {
        const auto& entry = metrics1[taskID];
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

    // Add summary section
    csvFile << "\nSummary\n\n";
    csvFile << "Metric,Value\n";

    // Retrieve summary metrics from TaskManager
    auto summaryMetrics = taskManager.metrics();
    auto memoryMetrics = taskManager.memoryManagerMetrics();

    
    // Write summary values to the CSV file
    csvFile << "Total Page Hits," << summaryMetrics[0] << "\n";
    csvFile << "Total Page Misses," << summaryMetrics[1] << "\n";
    csvFile << "Total Execution Time," << summaryMetrics[2] << "\n";
     csvFile << "\nMemory Manager Summary\n\n";
    double physicalMemoryGB = static_cast<double>(memoryMetrics[0]) / (1024 * 1024 * 1024);
    double virtualMemoryGB = static_cast<double>(memoryMetrics[1]) / (1024 * 1024 * 1024);
    csvFile << "Physical Memory (GB)," << physicalMemoryGB << "\n";
    csvFile << "Virtual Memory (GB)," << virtualMemoryGB << "\n";
    csvFile << "Page Size," << memoryMetrics[2] << " bytes\n";
    csvFile << "Total Physical Pages," << memoryMetrics[3] << "\n";
    csvFile << "Total Virtual Pages," << memoryMetrics[4] << "\n";
    csvFile << "Virtual Pages Available," << memoryMetrics[5] << "\n";
    csvFile << "Physical Pages Available," << memoryMetrics[6] << "\n";
    csvFile << "Physical Pages Allocated," << memoryMetrics[7] << "\n";

   

    csvFile.close();
    if (csvFile.fail()) {
        cerr << "Failed to write to file: " << filename << "\n";
    }
}

int main() {
    string file;
    cout << "ENTER FILE PATH" << endl;
    cin >> file;
    string arr[3]={"MAP","SINGLE_LEVEL_PAGE","MULTI_LEVEL_PAGE"};
    for(int i = 0; i < 3; i++) {
        TaskManager taskManager(i);
        Trace_file_task("./traces/" + file,taskManager);

        // Display the memory manager status
        cout << "file run successfull" << endl;
        
        // Generate a unique file name for each iteration
        string filename = "./output/" + arr[i]+ ".csv";
        
        // Write the metrics to a CSV file with the unique name
        writeMetricsToCSV(taskManager, filename); 
        cout << "Metrics written to " << filename << endl;
    }

    return 0;
}