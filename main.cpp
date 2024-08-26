#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <sstream>
#include <chrono>
#include "config.h"
//#include "task.cpp"

using namespace std;
using namespace std::chrono;

static int map_memory_hits = 0;
static int map_memory_misses = 0;

static int single_PT_memory_hits = 0;
static int single_PT_memory_misses = 0;

static int double_PT_memory_hits = 0;
static int double_PT_memory_misses = 0;

static int physical_page_number = 0;

class Memory_Manager
{
	public:
		long long int total_memory;
		int total_pages;
		int memory_allocated;
		int pages_allocated;
		long long int free_memory;
		int free_pages;
	
	Memory_Manager()
	{
		total_memory = pow(2,physical_memory_size);
		total_pages = pow(2,physical_memory_size)/pow(2,log2(page_size)+10);
		memory_allocated = 0;
		pages_allocated = 0;
		free_memory = total_memory - memory_allocated;
		free_pages = total_pages - pages_allocated;
	}
	
	//Status Functions - These functions update the attributes of the class 
	//and display them if needed.
	void update_memory_allocated(int pages);
	void update_pages_allocated(int pages);
	void update_free_memory();
	void update_free_pages();
	void print_allocator_status();
	
	//Conversion Functions - Hexadecimal to Decimal converison and vice-versa, 
	//to convert the hexadecimal logical addresses to decimal for easy increment 
	//and then converting them back to hexadecimal to give them 
	//an address format(0x<hexadecimal digits>).
	long int hex_to_dec(string logical_address);
	string dec_to_hex(int page_number);
	
	/*
	string dec_to_bin(int d_logical_address);
	int bin_to_dec(string page_number);
	*/
	
	//Increment Functions - Functions that will increment the decimal address 
	//values by page_size*1024
	void increment_physical_page_number();
	string increment_virtual_page_address(string address);
	
	//Formatting Function - Function that will add padding(0's in the front of the 
	//address value if is not eight digits)
	string padding(string hex_address);
	
	//Utility Functions - Functions that will call the above functions to 
	//get address incremented and stored and returned.
	string get_next_virtual_address(string virtual_address);
	string get_physical_address();
	vector<vector<string>> assign_memory(string virtual_page_address, int size);
};

void Memory_Manager::update_memory_allocated(int pages)
{
	memory_allocated += pages * pow(2,log2(page_size)+10);
}

void Memory_Manager::update_pages_allocated(int pages)
{
	pages_allocated += pages;
}

void Memory_Manager::update_free_memory()
{
	free_memory = total_memory - memory_allocated;
}

void Memory_Manager::update_free_pages()
{
	free_pages = total_pages - pages_allocated;
}

void Memory_Manager::print_allocator_status()
{
	cout<<"Total Memory Remaining = "<<total_memory - memory_allocated<<" bytes"<<endl;
	cout<<"Total Pages Remaining = "<<total_pages - pages_allocated<<"pages"<<endl;
	cout<<"Memory Allocated = "<<memory_allocated<<endl;
	cout<<"Pages Allocated = "<<pages_allocated<<endl;
	cout<<endl;
}

long int Memory_Manager::hex_to_dec(string logical_address)
{
	long int decimal_value = 0;
	decimal_value = stoi(logical_address,nullptr,16);
	return decimal_value;
}

string Memory_Manager::dec_to_hex(int address)
{
	string hex_addr = "";
	int current_address = address;
	while(current_address != 0)
	{
		int rem = 0;
		rem = current_address%16;
		if(rem==10)
		{
			hex_addr = "A" + hex_addr;
		}
		else if(rem==11)
		{
			hex_addr = "B" + hex_addr;
		}
		else if(rem==12)
		{
			hex_addr = "C" + hex_addr;
		}
		else if(rem==13)
		{
			hex_addr = "D" + hex_addr;
		}
		else if(rem==14)
		{
			hex_addr = "E" + hex_addr;
		}
		else if(rem==15)
		{
			hex_addr = "F" + hex_addr;
		}
		else
		{
			hex_addr = to_string(rem) + hex_addr;
		}
		current_address /=16;
	}
	return hex_addr;
}

void Memory_Manager::increment_physical_page_number()
{
	physical_page_number += 1;
}

string Memory_Manager::increment_virtual_page_address(string address)
{
	long int d_logical_address = hex_to_dec(address);
	d_logical_address += (page_size*1024);
	string new_address = dec_to_hex(d_logical_address);
	string logical_address = new_address;
	for(int i=0;i<(8-new_address.length()); i++)
	{
		logical_address = "0" + new_address;
	}
	logical_address = "0x"+logical_address;
	return logical_address;
}

string Memory_Manager::padding(string hex_address)
{
	int length = hex_address.length();
	int hex_address_length = (address_size/4); //need to get ceil but keeping it like this for now.
	for(int i=0;i<(hex_address_length-length);i++)
	{
		hex_address = "0" + hex_address;
	}
	hex_address = "0x" + hex_address;
	return hex_address;
}

string Memory_Manager::get_next_virtual_address(string virtual_address)
{
	//virtual_address is hexadecimal.
        long int dec_virtual_address = hex_to_dec(virtual_address);
	dec_virtual_address += page_size*1024;
	string new_virtual_address = dec_to_hex(dec_virtual_address);
	new_virtual_address = padding(new_virtual_address);
	return new_virtual_address;
}

string Memory_Manager::get_physical_address()
{
	long int dec_physical_address = physical_page_number*page_size*1024;
	string hex_physical_address = dec_to_hex(dec_physical_address);
	string physical_address = padding(hex_physical_address);
	increment_physical_page_number();
	return physical_address;
}

vector<vector<string>> Memory_Manager::assign_memory(string virtual_page_address, int size)
{
	//cout<<endl<<"In Memory Manager Module."<<endl;
	//cout<<"Number of pages to be allocated = "<<size<<endl;
	int num_pages;
	if(size%page_size == 0)
	{
		num_pages = (size/page_size);
	}
	else
	{
		num_pages = (size/page_size) + 1;
	}
	
	//updating the attributes of the class Memory_Manager.
	update_memory_allocated(num_pages);
	update_pages_allocated(num_pages);
	
	//defining a vector for storing 
	vector<string> logical_addresses;
	vector<string> physical_addresses;

	for(int i=0;i<num_pages;i++)
	{
		logical_addresses.push_back(virtual_page_address);
		string physical_address = get_physical_address();
		
		physical_addresses.push_back(physical_address);
		virtual_page_address = get_next_virtual_address(virtual_page_address);
	}
	vector<vector<string>> address;
	address.push_back(logical_addresses);
	address.push_back(physical_addresses);
	return address;
}

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
            multi_level_page_table.resize(4*1024);
            for(int i=0;i<multi_level_page_table.size();i++)
            {
            	multi_level_page_table[i].resize(1024);
            }
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
	void doublePT_check_physical_memory_allocated(Task& task, string logical_address, int size);
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
		long int dec_logical_address = hex_to_dec(logical_address);
		long int page_number = dec_logical_address/(page_size*1024);
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
	int pages;
	if(size%page_size == 0)
	{
		pages = (size/page_size);
	}
	else
	{
		pages = (size/page_size)+1;
	}
	//cout<<"Pages = "<<pages<<endl;
	//int level_one_index = stoi(logical_address.substr(0, 2), nullptr, 16);
	//int level_two_index = stoi(logical_address.substr(2), nullptr, 16);
	for(int i=0;i<pages;i++)
	{	
		long int dec_logical_address = hex_to_dec(logical_address);
		long int page_number = dec_logical_address/(page_size*1024);
		long int level_one_index = page_number/1024;
		long int level_two_index = page_number&((1<<10) - 1);
		//cout<<"Page Number = "<<page_number<<endl;
		//cout<<"Level One Index = "<<level_one_index<<endl;
		//cout<<"Level Two Index = "<<level_two_index<<endl;
		if(!task.multi_level_page_table[level_one_index][level_two_index].empty())
		{
			//cout<<endl<<"PAGE HIT!!"<<endl;
			double_PT_memory_hits += 1;
			task.dpt_hit += 1;
		}
		else
		{
			//cout<<"PAGE MISS"<<endl;
			double_PT_memory_misses += 1;
			task.dpt_miss += 1;
			vector<vector<string>> addresses = assign_memory(logical_address, page_size);
			task.multi_level_page_table[level_one_index][level_two_index] = addresses[1][0];
		}
		logical_address = get_next_virtual_address(logical_address);
	}
}


class IO
{    
    public:
    	Performance performance;
    	unordered_map<string, Task> tasks;
    	
    	void get_trace_map();
   	void process_trace_map(string t);
	void get_trace_spt();
   	void process_trace_spt(string t);
	void get_trace_dpt();
   	void process_trace_dpt(string t);
	void display_task_details_map();
	void display_task_details_spt();
	void display_task_details_dpt();
};

void IO::get_trace_map()
{
	ifstream traces("tracefile_4KB_4GB_4GB.txt");
	string t;

	while (getline(traces, t))
	{ 
	    process_trace_map(t);
	}
	traces.close();
}

void IO::process_trace_map(string t)
{
    	//cout<<"Trace - "<<t<<endl;
        string task_number, logical_address, size, size_number;        
        int first_col = t.find(":");
        int second_col = t.find(":",first_col+1);
        
        task_number =  t.substr(0,first_col);
        logical_address = t.substr(first_col+1,second_col-first_col-1);
        size = t.substr(second_col+1);
        //cout<<"Size = "<<size<<endl;
        //cout<<"Type of size = "<<typeid(size).name()<<endl;
        size_number = size.substr(0,2);
        stringstream ss;
        ss<<size_number;
        int allocation_size;
        ss>>allocation_size;

        if(tasks.find(task_number)==tasks.end()){
        //cout<<"added to tasks"<<endl;
        tasks[task_number] = Task(task_number);
        }
        //cout<<"size = "<<allocation_size+1<<endl;
        
        performance.map_check_physical_memory_allocated(tasks[task_number],logical_address,allocation_size);
        
        //tasks[task_number].map_add(logical_address,stoi(size_number));
        //tasks[task_number].single_level_add(logical_address, stoi(size_number));
}

void IO::get_trace_spt()
{
	ifstream traces("tracefile_4KB_4GB_4GB.txt");
	string t;

	while (getline(traces, t))
	{ 
	    process_trace_spt(t);
	}
	traces.close();
}

void IO::process_trace_spt(string t)
{
    	//cout<<"Trace - "<<t<<endl;
        string task_number, logical_address, size, size_number;        
        int first_col = t.find(":");
        int second_col = t.find(":",first_col+1);
        
        task_number =  t.substr(0,first_col);
        logical_address = t.substr(first_col+1,second_col-first_col-1);
        size = t.substr(second_col+1);
        //cout<<"Size = "<<size<<endl;
        //cout<<"Type of size = "<<typeid(size).name()<<endl;
        size_number = size.substr(0,2);
        stringstream ss;
        ss<<size_number;
        int allocation_size;
        ss>>allocation_size;

        if(tasks.find(task_number)==tasks.end()){
        //cout<<"added to tasks"<<endl;
        tasks[task_number] = Task(task_number);
        }
        //cout<<"size = "<<allocation_size+1<<endl;
        
        performance.singlePT_check_physical_memory_allocated(tasks[task_number],logical_address,allocation_size);
        
        //tasks[task_number].map_add(logical_address,stoi(size_number));
        //tasks[task_number].single_level_add(logical_address, stoi(size_number));
}

void IO::get_trace_dpt()
{
	ifstream traces("tracefile_4KB_4GB_4GB.txt");
	string t;

	while (getline(traces, t))
	{ 
	    process_trace_dpt(t);
	}
	traces.close();
}

void IO::process_trace_dpt(string t)
{
    	//cout<<"Trace - "<<t<<endl;
        string task_number, logical_address, size, size_number;        
        int first_col = t.find(":");
        int second_col = t.find(":",first_col+1);
        
        task_number =  t.substr(0,first_col);
        logical_address = t.substr(first_col+1,second_col-first_col-1);
        size = t.substr(second_col+1);
        //cout<<"Size = "<<size<<endl;
        //cout<<"Type of size = "<<typeid(size).name()<<endl;
        size_number = size.substr(0,2);
        stringstream ss;
        ss<<size_number;
        int allocation_size;
        ss>>allocation_size;

        if(tasks.find(task_number)==tasks.end()){
        //cout<<"added to tasks"<<endl;
        tasks[task_number] = Task(task_number);
        }
        //cout<<"size = "<<allocation_size+1<<endl;
        
        performance.doublePT_check_physical_memory_allocated(tasks[task_number],logical_address,allocation_size);
        
        //tasks[task_number].map_add(logical_address,stoi(size_number));
        //tasks[task_number].single_level_add(logical_address, stoi(size_number));
}

void IO::display_task_details_map()
{
	int t_size = tasks.size();
	for (auto& it: tasks)
	{
		cout << "Hits and Misses for "<<it.first<<endl;
		cout << "Hits = " << it.second.map_hit<<endl;
		cout << "Miss = " << it.second.map_miss<<endl;
	}
	cout<<"Total Hits - "<<map_memory_hits<<endl;
	cout<<"Total Misses - "<<map_memory_misses<<endl;
}
	
void IO::display_task_details_spt()
{
	int t_size = tasks.size();
	for (auto& it: tasks)
	{
		cout << "Hits and Misses for "<<it.first<<endl;
		cout << "Hits = " << it.second.spt_hit<<endl;
		cout << "Miss = " << it.second.spt_miss<<endl;
	}
	cout<<"Total Hits - "<<single_PT_memory_hits<<endl;
	cout<<"Total Misses - "<<single_PT_memory_misses<<endl;
    
}
	
void IO::display_task_details_dpt()
{
	int t_size = tasks.size();
	for (auto& it: tasks)
	{
		cout << "Hits and Misses for "<<it.first<<endl;
		cout << "Hits = " << it.second.dpt_hit<<endl;
		cout << "Miss = " << it.second.dpt_miss<<endl;
	}
	cout<<"Total Hits - "<<double_PT_memory_hits<<endl;
	cout<<"Total Misses - "<<double_PT_memory_misses<<endl;
}

int main() {
    IO ioManager;
    auto start_m = high_resolution_clock::now();
    ioManager.get_trace_map();
    auto stop_m = high_resolution_clock::now();
        auto duration_m = duration_cast<microseconds>(stop_m - start_m);
        cout<<"The execution time for Map Implementation = "<<duration_m.count()<<endl;
       
    cout<<endl<<"Map Implementation"<<endl;
    ioManager.display_task_details_map();
    
    auto start_s = high_resolution_clock::now();
    ioManager.get_trace_spt();
    auto stop_s = high_resolution_clock::now();
        auto duration_s = duration_cast<microseconds>(stop_s - start_s);
        cout<<"The execution time for Map Implementation = "<<duration_s.count()<<endl;
       
    cout<<endl<<"Single Level Page Table Implementation"<<endl;
    ioManager.display_task_details_spt();
        
    auto start_d = high_resolution_clock::now();
    ioManager.get_trace_dpt();
    auto stop_d = high_resolution_clock::now();
        auto duration_d = duration_cast<microseconds>(stop_d - start_d);
        cout<<"The execution time for Map Implementation = "<<duration_d.count()<<endl;
            
    cout<<endl<<"Multi Level Page Table Implementation"<<endl;
    ioManager.display_task_details_dpt();
    
    //cout<<endl<<count;
    return 0;
}
