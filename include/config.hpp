// config.h
#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>
#include <bits/stdc++.h>
#include <cmath>

size_t n = 32;
size_t m = 32;
size_t p = 12;


size_t pageSize = (1LL<<p);
size_t virtualMemorySize = (1LL<<n);
size_t physicalMemorySize = (1LL<<m);


// single level page table
size_t virtualPages = virtualMemorySize/pageSize;
size_t physicalPages = physicalMemorySize/pageSize;


// multi level page table
size_t pageTableSize2 = 1 << (int) ceil((n-p)/2);
size_t pageTableSize1 = 1 << (int) floor((n-p)/2);
size_t pteSize1 = n;
size_t pteSize2 = m;


// starting address of memory sections
size_t Text = 0x08048000;
size_t Data = 0x0ab11000;
size_t Bss = 0x0ab99000;
size_t Heap = 0x50000000;
size_t Shared = 0x40000000;
size_t Stack = 0xbfffb000;

size_t rangeText = Data - Text;
size_t rangeData = Bss - Data;
size_t rangeBss = Heap - Bss;
size_t rangeHeap = Shared - Heap;
size_t rangeShared = Stack - Shared;
size_t rangeStack = 0xffffffff - Stack;

#endif
