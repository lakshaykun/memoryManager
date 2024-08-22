#include <iostream>
using namespace std;

class MemoryManager {
private:
    size_t physical_memory;
    size_t virtual_memory;
    size_t page_size;
    int physical_pages=physical_memory/page_size;
    int virtual_pages=virtual_memory/page_size;


public:
    // Constructor
    MemoryManager(size_t phy, size_t vir, size_t page)
        : physical_memory(phy), virtual_memory(vir), page_size(page) {}

    // Method to display memory information
    void displayMemory() const {
        cout << "Physical Memory: " << physical_memory << " bytes" << endl;
        cout << "Virtual Memory: " << virtual_memory << " bytes" << endl;
         cout << "Physical page: " << physical_pages << " bytes" << endl;
        cout << "Virtual page: " << virtual_pages << " bytes" << endl;
        cout << "Page Size: " << page_size << " bytes" << endl;
    }
    void addtask(string start_addres,int size){
        int number_physical_page=size/page_size;
        physical_pages-=number_physical_page;

    }
};

int main() {
    // Create a MemoryManager object with example values
    MemoryManager manager(1000, 1000, 8);
    // Display memory information
    manager.displayMemory();
    manager.addtask("jasj",20);
    manager.displayMemory();
    return 0;
}
