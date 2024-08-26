#include <iostream>
#include <string>
#include <map>
#include "config.h"
//#include "task.cpp"

using namespace std;

extern int page_size;
extern int logical_memory_size;
extern int physical_memory_size;
extern int address_size; //number of bits for physical address.
extern int page_entry_size; //number of bytes required for each page entry.

static int map_memory_hits = 0;
static int map_memory_misses = 0;

static int single_PT_memory_hits = 0;
static int single_PT_memory_misses = 0;


class Task{
    string task_number, physical_address, logical_multi;
    unordered_map<string, string> map_page_table;
    vector<string> single_level_page_table;
    vector<vector<string>> multi_level_page_table;
    Memory_Manager mem_man;
    int page_size = 4;

    public:
    	int map_hit = 0;
    	int map_miss = 0;
    	int spt_hit = 0;
    	int spt_miss = 0;
    	int dpt_hit = 0;
    	int dpt_miss = 0;

        Task() : task_number("") {
        }
        Task(string n): task_number(n){
            single_level_page_table.resize(4*1024*1024);
        }
        
	friend class Performance;
	//friend class IO;
	
        void map_add(string logical_address, int size){
            if(map_page_table.find(logical_address)!=map_page_table.end()){
                //cout<<endl<<"PAGE HIT!!"<<endl;
                map_hit+=1;
            }
            else{
            	map_miss+=1;
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

        /*void single_level_add(string logical_address, int size){
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
    }
        }*/

        /*void multi_level_add(string logical_address, int size){
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
        }*/

};


class Performance : public Memory_Manager
{
	public:
		Memory_Manager memory;
	
	int calculate_page_table_size();
	long int hex_to_dec(string logical_address);
	string get_next_virtual_address(string logical_address);
	vector<vector<string>> assign_memory(string virtual_page_address, int size);
	void map_check_physical_memory_allocated(Task& task, string logical_address, int size);
	void singlePT_check_physical_memory_allocated(Task& task, string logical_address, int size);
};

int Performance::calculate_page_table_size()
{
	int num_pages = 0;
	int page_table_size = 0;
	
	//number of pages for every task's page table.
	num_pages = logical_memory_size/page_size;
	
	//size of page table in bytes.
	page_table_size = num_pages*page_entry_size;
	
	return page_table_size;
}

long int Performance::hex_to_dec(string logical_address)
{
	Memory_Manager m;
	long int decimal_value = m.hex_to_dec(logical_address);
	return decimal_value;
}

string Performance::get_next_virtual_address(string virtual_address)
{
	Memory_Manager m;
	string new_virtual_address = m.get_next_virtual_address(virtual_address);
	return new_virtual_address;
}

vector<vector<string>> Performance::assign_memory(string virtual_page_address, int size)
{
	Memory_Manager m;
	vector<vector<string>> address = m.assign_memory(virtual_page_address, size);
	return address;
}


void Performance::map_check_physical_memory_allocated(Task& task, string logical_address, int size)
{

	int pages;
	if(size%page_size == 0)
	{
		pages = (size/page_size);
	}
	else
	{
		pages = (size/page_size)+1;
	}
	
	for(int i=0;i<pages;i++)
	{
		if(task.map_page_table.find(logical_address) != task.map_page_table.end())
		{
			map_memory_hits += 1;
			task.map_hit += 1;
		}
		else
		{
			map_memory_misses += 1;
			task.map_miss += 1;
			vector<vector<string>> addresses = assign_memory(logical_address, page_size);
			task.map_page_table[addresses[0][0]] = addresses[1][0];
		}
		logical_address = get_next_virtual_address(logical_address);
	}
}

void Performance::singlePT_check_physical_memory_allocated(Task& task, string logical_address, int size)
{
	long int dec_logical_address = hex_to_dec(logical_address);
	long int page_number = dec_logical_address/(page_size*1024);
	
	int pages;
	if(size%page_size == 0)
	{
		pages = (size/page_size);
	}
	else
	{
		pages = (size/page_size)+1;
	}
	
	for(int i=0;i<pages;i++)
	{
		if(task.single_level_page_table[page_number]!="")
		{
			single_PT_memory_hits += 1;
			task.spt_hit += 1;
		}
		else
		{
			single_PT_memory_misses += 1;
			task.spt_miss += 1;
			vector<vector<string>> addresses = assign_memory(logical_address, page_size);
			task.single_level_page_table[page_number] = addresses[1][0];
		}
		logical_address = get_next_virtual_address(logical_address);
	}
	
	//cout<<"The value at the 0th index in single level page table is = "<<typeid(task.single_level_page_table[0]).name()<<endl;
	//basically do sth similar to how the map implementation is done.
}

void Performance::doublePT_check_physical_memory_allocated(Task& task, string logical_address, int size)
{
	
}
