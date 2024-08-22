// MemoryManager.cpp
#include "../include/MemoryManager.h"
#include <iostream>

MemoryManager::MemoryManager(size_t totalPages)
    : totalPhysicalPages(totalPages), pageStatus(totalPages, false) {}

int MemoryManager::allocatePage() {
    for (size_t i = 0; i < totalPhysicalPages; ++i) {
        if (!pageStatus[i]) {
            pageStatus[i] = true;
            return i; // Return the physical page index
        }
    }
    return -1; // No free page available
}

void MemoryManager::deallocatePage(int pageIndex) {
    if (pageIndex >= 0 && pageIndex < totalPhysicalPages) {
        pageStatus[pageIndex] = false;
    }
}

size_t MemoryManager::getFreePages() const {
    size_t freeCount = 0;
    for (const auto& status : pageStatus) {
        if (!status) ++freeCount;
    }
    return freeCount;
}
