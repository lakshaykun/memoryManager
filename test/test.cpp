#include <iostream>
#include <fstream>
#include <cstdlib>  
#include <ctime>   
#include "../include/config.hpp" 
using namespace std;

// Function to generate random addresses aligned to the page size
unsigned int generateRandomAddress(unsigned int start, unsigned int range) {
    unsigned int randomOffset = (rand() % (range / pageSize)) * pageSize;
    return start + randomOffset;
}

// Function to generate a random task size that is a multiple of 4
unsigned int generateTaskSize(int size) {
    // Generate task sizes that are multiples of 4 bytes
    return (rand() % 16 + 1) * size; 
}

int main() {
    int n;
    cout<<"Enter the number of traces line"<<endl;
    cin>>n;
   
    srand(static_cast<unsigned int>(time(0)));

   
    int numTasks ;// You can change this as needed
    cout<<"Enter number of task"<<endl;
    cin>>numTasks;
    
    cout<<"File size in KB-->1 or MB-->2"<<endl;
    int kbormb;
    cin>>kbormb;
    string tasksize;
    if(kbormb==1){
        tasksize="KB";
    }
    else{
        tasksize="MB";
    }
    cout<<"Enter minimum file size number only"<<endl;
    int size;
    cin>>size;

    // Open a file to write the generated traces
    std::ofstream traceFile("./traces/trace.txt");
    if (!traceFile) {
        std::cerr << "Error opening file for writing traces!" << std::endl;
        return 1;
    }

 
    for (int i = 0; i < n; ++i) {
        
        unsigned int startAddress;
        unsigned int range;
        int randomNumber = rand() % numTasks + 1;
        std::string sectionName;

        int sectionType = rand() % 5;
        switch (sectionType) {
            case 0:
                startAddress = Text;
                range = rangeText;
                sectionName = "Text";
                break;
            case 1:
                startAddress = Data;
                range = rangeData;
                sectionName = "Data";
                break;
            case 2:
                startAddress =Stack;
                range = rangeStack;
                sectionName = "Stack";
                break;
            case 3:
                startAddress = Shared;
                range = rangeShared;
                sectionName = "SHARED";
                break;
            case 4:
                startAddress = Heap;
                range = rangeHeap;
                sectionName = "Heap";
                break;
             case 5:
                startAddress = Bss;
                range =rangeBss;
                sectionName = "Bss";
                break;
        }

        unsigned int address = generateRandomAddress(startAddress, range);
        unsigned int taskSize = generateTaskSize(size);

    
        traceFile << "T" << randomNumber
                 
                  << ":0x" << std::setw(8) << std::setfill('0') <<std::hex << address 
                  << ":" << std::dec << taskSize << tasksize<<"\n";
    }

    traceFile.close();
    std::cout << "Memory access traces generated and written to memory_traces.txt" << std::endl;
    return 0;
}
