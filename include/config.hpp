// config.h
#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>
#include <bits/stdc++.h>

size_t addr = 32;

size_t n = 33;
size_t m = 34;
size_t p = 10;


size_t pageSize = (1LL<<p);
size_t virtualMemorySize = (1LL<<n);
size_t physicalMemorySize = (1LL<<m);


// single level page table
size_t virtualPages = virtualMemorySize/pageSize;
size_t physicalPages = physicalMemorySize/pageSize;


// multi level page table
size_t pageTableSize2 = 1 << (n-p)/2;
size_t pageTableSize1 = 1 << (n-p-pageTableSize2);
size_t pageTableEntrySize1 = 1 << (n-p-pageTableSize2);
size_t pageTableEntrySize2 = 1 << (n-p)/2;


// starting address of memory sections
size_t Text = 0x08048000;
size_t Data = 0x0ab11000;
size_t Bss = 0x0ab99000;
size_t Heap = 0x50000000;
size_t Shared = 0x40000000;
size_t Stack = 0xbfffb000;

#endif
