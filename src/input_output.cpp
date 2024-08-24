#include<bits/stdc++.h>

#include "../include/taskManager.hpp"
#include "../include/task.hpp"
#include "../include/config.hpp"

using namespace std;
using namespace std;
TaskManager taskManager;
void Trace_file_task(const string& filename){
    ifstream file(filename);
    if(!file.is_open()){
        cout<<"Error While Loading File"<<endl;
        return;
    }
    string line;
    while(getline(file,line)){
        if(line.empty()|| line.find(':')==string::npos){
            continue;
        }
        string taskid;
        string address;
        string size;

        istringstream iss(line);
        getline(iss,taskid,':');
        getline(iss,address,':');
        getline(iss,size);
       
        address=address.substr(2);
        size_t address_hex=stoull(address,nullptr,16);
       
        size_t size_dec=stoull(size.substr(0,size.find("KB")))*1024;
        taskManager.addTask(taskid, address_hex, size_dec);
        
        


    }
}

int main(){
    Trace_file_task("memory_traces.txt");
    
    // Create a TaskManager object
    
    // Add a task to the TaskManager
    taskManager.displayMemoryManager();
    taskManager.displayPageHits();
    return 0;
}