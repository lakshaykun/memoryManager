#ifndef MEMORY_MANAGER
#define MEMORY_MANAGER
#include <iostream>
#include <vector>
using namespace std;

class MemoryManager {
private:
    size_t physical_memory;
    size_t virtual_memory;
    size_t page_size;
    size_t physical_pages_available;
    size_t virtual_pages;
    size_t physical_pages_allocated;
    size_t physical_pages;
    size_t current_phy_page;
    std::vector<int> task;
    std::vector<bool> arr;

public:
    // Constructor
    MemoryManager(size_t phy, size_t vir, size_t page)
        : physical_memory(phy),
          virtual_memory(vir),
          page_size(page),
          physical_pages_available(phy / page),
          physical_pages(phy / page),
          virtual_pages(vir / page),
          physical_pages_allocated(0),
          current_phy_page(0),
          arr(phy / page, false) // Initialize vector arr with physical_pages elements set to false
    {
        // Constructor body (if needed)
    }
   
    long long allocate_memory(){
        if(physical_pages_available>=1){
            physical_pages_allocated+=1;
            physical_pages_available-=1;
            physical_memory-=page_size;
            
            if(current_phy_page<physical_pages){
                arr[current_phy_page]=1;
                current_phy_page+=1;
                return current_phy_page-1;
            }
            else{
                for(size_t i=0;i<physical_pages;i++){
                    if(arr[i]==0){
                        arr[i]=1;
                        return i;
                    }
                }
            }

        }
        else{
            return -1;
        }
    }
    void deallocatePage(int page){
      physical_pages_allocated-=1;
            physical_pages_available+=1;
            physical_memory+=page_size;
            arr[page]=0;
            

    }
};

#endif // MEMORY_MANAGER
