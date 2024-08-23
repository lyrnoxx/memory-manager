#include<iostream>
#include<fstream>
#include<map>
#include<unordered_map>
#include<vector>
using namespace std;

class Memory_Manager{
    public:
        vector<vector<string>> assign_memory(string a, string b){
            vector<string> frames;
            vector<string> pages;
            vector<vector<string>> combined;
            for (int i = 0; i < 3; ++i) {
            frames.push_back("9763334746");
            }
            for (int i = 0; i < 3; ++i) {
            pages.push_back("32543546");
            }
            combined.push_back(frames);
            combined.push_back(pages);
            return combined;
        }
};

class Task{
    string task_number, physical_address, logical_multi;
    unordered_map<string, string> map_page_table;
    /*
    vector<string, vector<string>> single_level_page_table;
    unordered_map<string, string> multi_level_page_table;
    */
    Memory_Manager mem_man;
    public:
        Task() : task_number("") {}
        Task(string n): task_number(n){}

        void map_add(string logical_address, string size){
            if(map_page_table.find(logical_address)!=map_page_table.end()){
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
            for (const auto& entry : map_page_table) {
        cout << "Logical Address: " << entry.first << " -> Physical Frames: ";
        for (const auto& frame : entry.second) {
            cout << frame << " ";
        }
        cout << endl;
    }
        }
/*
        void single_level_add(string logical_address, string size){
            if(map_page_table.find(logical_address)!=map_page_table.end()){
                cout<<endl<<"PAGE HIT!!"<<endl;
            }
            else{
                vector<string> physical_addresses = mem_man.assign_memory(logical_address, size);
                map_page_table[logical_address] = physical_addresses;
            }
        }

        void multi_level_add(string logical_address, string size){
            if(map_page_table.find(logical_address)!=map_page_table.end()){
                cout<<endl<<"PAGE HIT!!"<<endl;
            }
            else{
                vector<string> physical_addresses = mem_man.assign_memory(logical_address, size);
                map_page_table[logical_address] = physical_addresses;
            }
        }
*/
};

class IO{
    unordered_map<string, Task> tasks;
    public:
    void get_trace(){
        ifstream traces("traces.txt");
        string t;
    
        while (getline(traces, t)) { 
            process_trace(t);
        } 
        traces.close();
    }

    void process_trace(string t){
        string task_number, logical_address, size;        
        int first_col = t.find(":");
        int second_col = t.find(":",first_col+1);
        
        task_number =  t.substr(0,first_col);
        logical_address = t.substr(first_col+1,second_col-first_col-1);
        size = t.substr(second_col+1);

        if(tasks.find(task_number)==tasks.end()){
        tasks[task_number] = Task(task_number);
        }
        tasks[task_number].map_add(logical_address,size);
    }

};


int main() {
    IO ioManager;
    ioManager.get_trace();

    return 0;
}