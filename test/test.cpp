#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <random>
#include "../include/config.hpp"

using namespace std;

// Structure to pass data to thread function
struct TaskData {
    int taskId;
    int size;
    string taskSizeUnit;
    int tracesToGenerate;
};

// Function to generate random addresses aligned to the page size
unsigned int generateRandomAddress(unsigned int start, unsigned int range) {
    unsigned int randomOffset = (rand() % (range / pageSize)) * pageSize;
    return start + randomOffset;
}

// Function to generate a random task size that is a multiple of 4
unsigned int generateTaskSize(int size) {
    return (rand() % 16 + 1) * size;
}

// Thread function to generate traces
void* generateTrace(void* arg) {
    TaskData* taskData = (TaskData*)arg;

    // Create a unique file for each thread
    string filename = "./traces/trace_" + to_string(taskData->taskId) + ".txt";
    ofstream traceFile(filename);
    if (!traceFile) {
        cerr << "Error opening file for writing traces: " << filename << endl;
        pthread_exit(NULL);
    }

    for (int i = 0; i < taskData->tracesToGenerate; ++i) {
        unsigned int startAddress;
        unsigned int range;

        int sectionType = rand() % 6;

        // Select a section to generate the trace
        switch (sectionType) {
            case 0: startAddress = Text; range = rangeText; break;
            case 1: startAddress = Data; range = rangeData; break;
            case 2: startAddress = Stack; range = rangeStack; break;
            case 3: startAddress = Shared; range = rangeShared; break;
            case 4: startAddress = Heap; range = rangeHeap; break;
            case 5: startAddress = Bss; range = rangeBss; break;
        }

        // Generate address and task size
        unsigned int taskSize = generateTaskSize(taskData->size);
        unsigned int address = generateRandomAddress(startAddress, range - taskSize * 1024 * (taskData->taskSizeUnit == "KB" ? 8 : 1024 * 8));

        // Write the generated trace to the thread-specific file
        traceFile << "T" << taskData->taskId
                  << ":0x" << setw(8) << setfill('0') << hex << address
                  << ":" << dec << taskSize << taskData->taskSizeUnit << "\n";
    }

    traceFile.close();
    pthread_exit(NULL);
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    int n, numTasks, size, kbormb;
    string taskSizeUnit;

    cout << "Enter the number of trace lines: ";
    cin >> n;
    cout << "Enter number of tasks: ";
    cin >> numTasks;
    cout << "File size in KB-->1 or MB-->2: ";
    cin >> kbormb;
    taskSizeUnit = (kbormb == 1) ? "KB" : "MB";
    cout << "Enter minimum file size number only: ";
    cin >> size;

    // Calculate base traces per task
    vector<int> tracesPerTask(numTasks);
    int baseTracesPerTask = n / numTasks;
    int remainingTraces = n % numTasks;

    for (int i = 0; i < numTasks; ++i) {
        tracesPerTask[i] = baseTracesPerTask;
    }

    // Distribute remaining traces
    for (int i = 0; i < remainingTraces; ++i) {
        tracesPerTask[i]++;
    }

    // Create threads
    vector<pthread_t> threads(numTasks);
    vector<TaskData> taskData(numTasks);

    for (int i = 0; i < numTasks; ++i) {
        taskData[i] = {i + 1, size, taskSizeUnit, tracesPerTask[i]};

        int rc = pthread_create(&threads[i], NULL, generateTrace, (void*)&taskData[i]);
        if (rc) {
            cerr << "Error: unable to create thread, " << rc << endl;
            exit(-1);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < numTasks; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Collect all lines from the thread-specific files into a vector
    vector<string> allTraces;
    for (int i = 1; i <= numTasks; ++i) {
        string filename = "./traces/trace_" + to_string(i) + ".txt";
        ifstream infile(filename);
        if (!infile) {
            cerr << "Error opening file for reading: " << filename << endl;
            continue;
        }

        string line;
        while (getline(infile, line)) {
            allTraces.push_back(line);
        }
        infile.close();
        remove(filename.c_str()); // Optionally remove the temporary file after merging
    }

    // Shuffle the collected traces randomly
    random_device rd;
    mt19937 g(rd());
    shuffle(allTraces.begin(), allTraces.end(), g);

    // Write all shuffled traces into the final trace file
    ofstream traceFile("./traces/trace.txt");
    if (!traceFile) {
        cerr << "Error opening final trace file for writing!" << endl;
        return 1;
    }

    for (const auto& trace : allTraces) {
        traceFile << trace << "\n";
    }

    traceFile.close();
    cout << "Memory access traces generated and written to trace.txt" << endl;

    return 0;
}
