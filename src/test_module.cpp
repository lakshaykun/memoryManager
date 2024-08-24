#include <iostream>
#include <fstream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include "../include/config.hpp" // Include the configuration file
using namespace std;

// Function to generate random addresses aligned to the page size
unsigned int generateRandomAddress(unsigned int start, unsigned int range) {
    unsigned int randomOffset = (rand() % (range / pageSize)) * pageSize;
    return start + randomOffset;
}

// Function to generate a random task size that is a multiple of 4
unsigned int generateTaskSize(int size) {
    // Generate task sizes that are multiples of 4 bytes
    return (rand() % 16 + 1) * size; // Example: Random sizes between 4 and 64 bytes (multiples of 4)
}

int main() {
    int n;
    cout<<"Enter the number of traces line"<<endl;
    cin>>n;
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Number of tasks (e.g., number of memory access traces)
    int numTasks ;// You can change this as needed
    cout<<"Enter number of task"<<endl;
    cin>>numTasks;
    cout<<"Enter minimum file size number only"<<endl;
    int size;
    cin>>size;
    cout<<"KB or MB"<<endl;
    string tasksize;
    cin>>tasksize;

    // Open a file to write the generated traces
    std::ofstream traceFile("memory_traces.txt");
    if (!traceFile) {
        std::cerr << "Error opening file for writing traces!" << std::endl;
        return 1;
    }

    // Define range for each section for generating addresses
    const unsigned int TEXT_RANGE = 0x1000;    // Example range for text section
    const unsigned int DATA_RANGE = 0x1000;    // Example range for data section
    const unsigned int STACK_RANGE = 0x1000;   // Example range for stack section
    const unsigned int SHARED = 0x1000; // Example range for shared library section
    const unsigned int HEAP_RANGE = 0x1000;    // Example range for heap section
    const unsigned int BSS = 0x1000;    // Example range for heap section

    // Generate traces
    for (int i = 0; i < n; ++i) {
        // Randomly choose a section for the task
        unsigned int startAddress;
        unsigned int range;
        int randomNumber = rand() % numTasks + 1;
        std::string sectionName;

        int sectionType = rand() % 5;
        switch (sectionType) {
            case 0:
                startAddress = Text;
                range = TEXT_RANGE;
                sectionName = "Text";
                break;
            case 1:
                startAddress = Data;
                range = DATA_RANGE;
                sectionName = "Data";
                break;
            case 2:
                startAddress =Stack;
                range = STACK_RANGE;
                sectionName = "Stack";
                break;
            case 3:
                startAddress = Shared;
                range = SHARED;
                sectionName = "SHARED";
                break;
            case 4:
                startAddress = Heap;
                range = HEAP_RANGE;
                sectionName = "Heap";
                break;
             case 5:
                startAddress = Bss;
                range = BSS;
                sectionName = "Bss";
                break;
        }

        unsigned int address = generateRandomAddress(startAddress, range);
        unsigned int taskSize = generateTaskSize(size);

        // Write the task ID, address, and task size to the trace file
        traceFile << "T" << randomNumber
                 
                  << ":0x" << std::hex << address 
                  << ":" << std::dec << taskSize << tasksize<<"\n";
    }

    traceFile.close();
    std::cout << "Memory access traces generated and written to memory_traces.txt" << std::endl;

    return 0;
}
