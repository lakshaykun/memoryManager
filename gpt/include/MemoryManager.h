// MemoryManager.hpp
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <vector>

class MemoryManager {
private:
    size_t totalPhysicalPages;
    std::vector<bool> pageStatus; // true = allocated, false = free

public:
    MemoryManager(size_t totalPages);
    int allocatePage();
    void deallocatePage(int pageIndex);
    size_t getFreePages() const;
};

#endif // MEMORY_MANAGER_H
