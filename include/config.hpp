// config.h
#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>

size_t pageSize = (1<<12);
size_t virtualMemorySize = (1<<20);
size_t physicalMemorySize = (1<<20);


// single level page table
size_t virtualPages = virtualMemorySize/pageSize;
size_t physicalPages = physicalMemorySize/pageSize;


// multi level page table
size_t pageTableSize1 = (1<<10);
size_t pageTableSize2 = (1<<10);

#endif
