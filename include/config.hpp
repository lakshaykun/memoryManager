// config.h
#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>

size_t addr = 32;

size_t n = 32;
size_t m = 32;
size_t p = 12;


size_t pageSize = (1<<p);
size_t virtualMemorySize = (1<<n);
size_t physicalMemorySize = (1<<m);


// single level page table
size_t virtualPages = virtualMemorySize/pageSize;
size_t physicalPages = physicalMemorySize/pageSize;


// multi level page table
size_t temp = n-p;
size_t pageTableSize1 = (1<<((n-p)/2));
size_t pageTableSize2 = (1<<((n-p)%2));

#endif
