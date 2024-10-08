#ifndef MEMORY_MANAGER
#define MEMORY_MANAGER

#include <iostream>
#include <vector>
#include <iomanip>  // For std::fixed and std::setprecision
using namespace std;

class MemoryManager {
private:
    size_t physical_memory;
    size_t virtual_memory;
    size_t page_size;
    size_t physical_pages_available;
    size_t virtual_pages;
    size_t virtual_pages_available;
    size_t physical_pages_allocated;
    size_t physical_pages;
    size_t current_phy_page;
    
    std::vector<bool> arr;

public:
    // Constructor
    MemoryManager(size_t phy, size_t vir, size_t page)
        : physical_memory(phy),
          virtual_memory(vir),
          page_size(page),
          physical_pages(phy / page),
          physical_pages_available(phy / page),
          virtual_pages(vir / page),
          physical_pages_allocated(0),
          current_phy_page(0),
          virtual_pages_available(virtual_memory / page_size),
          arr(phy / page, false) // Initialize vector arr with physical_pages elements set to false
    {}

    // destructor
    ~MemoryManager() {
        arr.clear();
    }

    // Function to allocate a page
    int allocatePage() {
        if (physical_pages_available > 0) {
            physical_pages_allocated++;
            physical_pages_available--;
            physical_memory -= page_size;

            if (current_phy_page < physical_pages) {
                arr[current_phy_page] = true;
                virtual_pages_available--;
                return current_phy_page++;
            } else {
                for (size_t i = 0; i < physical_pages; ++i) {
                    if (!arr[i]) {
                        arr[i] = true;
                        return i;
                    }
                }
            }
        }
        return -1; // No available page
    }

    // Function to deallocate a page
    void deallocatePage(int page) {
        if (page >= 0 && page < physical_pages && arr[page]) {
            physical_pages_allocated--;
            physical_pages_available++;
            physical_memory += page_size;
            arr[page] = false;
        } else {
            std::cerr << "Error: Invalid page number or page not allocated." << std::endl;
        }
    }

    // Function to get the number of physical pages allocated
    void displayMemory() const {
        // Convert bytes to gigabytes
        double physicalMemoryGB = static_cast<double>(physical_memory) / (1024 * 1024 * 1024);
        double virtualMemoryGB = static_cast<double>(virtual_memory) / (1024 * 1024 * 1024);

        // Print the results with 2 decimal places
        std::cout << "PHYSICAL MEMORY AVAILABLE: " << std::fixed << std::setprecision(17) << physicalMemoryGB << " GB" << std::endl;
        
        std::cout << "PHYSICAL PAGES ALLOCATED : " << physical_pages_allocated << std::endl;
        std::cout << "PHYSICAL PAGES AVAILABLE : " << physical_pages_available << std::endl;
    }

    vector<size_t> getMemoryStatus() const {
        return {physical_memory, virtual_memory, page_size, physical_pages, virtual_pages, virtual_pages_available, physical_pages_available, physical_pages_allocated, physicalMemorySize};
    }

};

#endif // MEMORY_MANAGER