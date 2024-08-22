// IO_Module.cpp
#include "../include/IO_Module.h"
#include <fstream>
#include <sstream>
#include <iostream>

void processTrace(const std::string& traceFile, MemoryManager& memManager, std::vector<Task>& tasks) {
    std::ifstream file(traceFile);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string taskId;
        std::string logicalAddressHex;
        std::string sizeStr;
        if (iss >> taskId >> logicalAddressHex >> sizeStr) {
            int logicalAddress = std::stoi(logicalAddressHex, nullptr, 16);
            size_t size = std::stoi(sizeStr.substr(0, sizeStr.size() - 2)); // Assume KB or MB

            for (auto& task : tasks) {
                if (task.getTaskId() == taskId) {
                    task.requestMemory(logicalAddress, size * 1024); // Convert size to bytes
                    break;
                }
            }
        }
    }
}
