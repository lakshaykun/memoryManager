// TestModule.cpp
#include "../include/TestModule.h"
#include <fstream>
#include <iomanip>
#include <iostream>

void generateTrace(int numTasks) {
    std::ofstream traceFile("trace.txt");
    for (int i = 1; i <= numTasks; ++i) {
        std::string taskId = "T" + std::to_string(i);
        int logicalAddress = 0x4000;
        int size = 16; // Assume 16KB
        traceFile << taskId << ": 0x" << std::hex << logicalAddress << ": " << size << "KB\n";
        logicalAddress += 0x2000;
        traceFile << taskId << ": 0x" << std::hex << logicalAddress << ": " << (size / 4) << "KB\n";
    }
    traceFile.close();
}