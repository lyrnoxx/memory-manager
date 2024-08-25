#include<iostream>
#include<fstream>
#include<map>
#include<unordered_map>
#include<vector>
#include "memory_manager_module.cpp"
#include "performance_module.cpp"
using namespace std;

static int count;

/*class Memory_Manager{
    public:
        vector<vector<string>> assign_memory(string a, int b){
            vector<string> frames;
            vector<string> pages;
            vector<vector<string>> combined;
            for (int i = 0; i < 3; ++i) {
            frames.push_back("0x03083400");
            }
            for (int i = 0; i < 3; ++i) {
            pages.push_back("32543546");
            }
            combined.push_back(frames);
            combined.push_back(pages);
            return combined;
        }
};*/

class Task{
    string task_number, physical_address, logical_multi;
    unordered_map<string, string> map_page_table;
    vector<string> single_level_page_table;
    vector<vector<string>> multi_level_page_table;
    Memory_Manager mem_man;
    int page_size = 4;

    public:
        Task() : task_number("") {
        }
        Task(string n): task_number(n){
            single_level_page_table.resize(4*1024*1024);
        }

        void map_add(string logical_address, int size){
            if(map_page_table.find(logical_address)!=map_page_table.end()){
                cout<<endl<<"PAGE HIT!!"<<endl;
                count+=1;
            }
            else{
                vector<vector<string>> combined_addresses = mem_man.assign_memory(logical_address, size);
                for (size_t i=0; i<combined_addresses[0].size();i++){
                    logical_multi = combined_addresses[0][i];
                    physical_address = combined_addresses[1][i];
                    map_page_table[logical_multi] = physical_address;
                }

            }
            /*for (const auto& entry : map_page_table) {
        cout << "Logical Address: " << entry.first << " -> Physical Frames: ";
        for (const auto& frame : entry.second) {
            cout << frame << " ";
        }
        cout << endl;
    }*/
    }

        void single_level_add(string logical_address, int size){
            size_t index=stoi(logical_address,nullptr,16) / (page_size*1024);
            if(!single_level_page_table[index].empty()){
                cout<<endl<<"PAGE HIT!!"<<endl;
            }
            else{
                vector<vector<string>> combined_addresses = mem_man.assign_memory(logical_address, size);
                for (size_t i=0; i<combined_addresses[0].size();i++){
                    int logical_pages = stoi(combined_addresses[0][i], nullptr, 16) / (page_size*1024);
                    physical_address = combined_addresses[1][i];
                    single_level_page_table[logical_pages] = physical_address;
                }
            }
           /* for (size_t i = 0; i < single_level_page_table.size(); i++) {
        if (!single_level_page_table[i].empty()) {
            cout << "Logical Page Number: " << i << " -> Physical Frame: " << single_level_page_table[i] << endl;
        }
    }*/
        }

        void multi_level_add(string logical_address, int size){
            int level_one_index = stoi(logical_address.substr(0, 2), nullptr, 16);
            int level_two_index = stoi(logical_address.substr(2), nullptr, 16);
            if(!multi_level_page_table[level_one_index][level_two_index].empty()){
                cout<<endl<<"PAGE HIT!!"<<endl;
            }
            else{
                vector<vector<string>> combined_addresses = mem_man.assign_memory(logical_address, size);
                for (size_t i=0; i<combined_addresses[0].size();i++){
                    logical_multi = combined_addresses[0][i];
                    physical_address = combined_addresses[1][i];
                    map_page_table[logical_multi] = physical_address;
                }
            }
        }

};

class IO{
    Performance performance;
    unordered_map<string, Task> tasks;
    public:

    void get_trace(){
        ifstream traces("tracefile_2KB_4GB_8GB.txt");
        string t;
    
        while (getline(traces, t)) { 
            process_trace(t);
        } 
        traces.close();
    }

    void process_trace(string t){
        string task_number, logical_address, size, size_number;        
        int first_col = t.find(":");
        int second_col = t.find(":",first_col+1);
        
        task_number =  t.substr(0,first_col);
        logical_address = t.substr(first_col+1,second_col-first_col-1);
        size = t.substr(second_col+1);

        for (char ch : size) {
        if (std::isdigit(ch)) {
            size_number += ch;
        } else {
            break;  
        }
    }

        if(tasks.find(task_number)==tasks.end()){
        tasks[task_number] = Task(task_number);
        }
        tasks[task_number].map_add(logical_address,stoi(size_number));
        //tasks[task_number].single_level_add(logical_address, stoi(size_number));
        performance.map_check_physical_memory_allocated(tasks[task_number],logical_address,stoi(size_number));
    }

};


int main() {
    IO ioManager;
    ioManager.get_trace();
    cout<<endl<<count;
    return 0;
}