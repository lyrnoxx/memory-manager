#include<iostream>
#include<fstream>
#include<map>
#include<unordered_map>
#include<vector>

using namespace std;

class Task{
    string task_number, physical_address, logical_multi;
    unordered_map<string, string> map_page_table;
    vector<string> single_level_page_table;
    vector<vector<string>> multi_level_page_table;
    Memory_Manager mem_man;
    int page_size = 4;

    public:
    	int hit = 0;
    	int miss = 0;
        Task() : task_number("") {
        }
        Task(string n): task_number(n){
            single_level_page_table.resize(4*1024*1024);
        }
        
	friend class Performance;
	friend class IO;
        void map_add(string logical_address, int size){
            if(map_page_table.find(logical_address)!=map_page_table.end()){
                //cout<<endl<<"PAGE HIT!!"<<endl;
                //hit+=1;
            }
            else{
            	//miss+=1;
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
