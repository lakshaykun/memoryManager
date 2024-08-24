// config.h
#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>
#include <bits/stdc++.h>

size_t addr = 32;

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
size_t pageTableSize2 = 1 << (n-p)/2;
size_t pageTableSize1 = 1 << (n-p-pageTableSize2);
size_t pageTableEntrySize1 = 1 << (n-p-pageTableSize2);
size_t pageTableEntrySize2 = 1 << (n-p)/2;


// starting address of memory sections
size_t text = 0x08048000;
size_t data = 0x0ab11000;
size_t bss = 0x0ab99000;
size_t heap = 0x50000000;
size_t shared = 0x40000000;
size_t stack = 0xbfffb000;

#endif
