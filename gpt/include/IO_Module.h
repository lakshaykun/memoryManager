// IO_Module.h
#ifndef IO_MODULE_H
#define IO_MODULE_H

#include "../include/Task.h"
#include <vector>
#include <string>

void processTrace(const std::string& traceFile, MemoryManager& memManager, std::vector<Task>& tasks);

#endif // IO_MODULE_H
