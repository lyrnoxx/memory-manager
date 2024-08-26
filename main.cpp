#include<iostream>
#include<fstream>
#include<map>
#include<unordered_map>
#include<vector>
#include<sstream>
#include "memory_manager_module.cpp"
#include "performance_module.cpp"
//#include "config.h"
//#include "task.cpp"

using namespace std;

static int count;
extern int map_memory_hits;
extern int map_memory_misses;

extern int single_PT_memory_hits;
extern int single_PT_memory_misses;

extern int double_PT_memory_hits;
extern int double_PT_memory_misses;

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



class IO{
    Performance performance;
    unordered_map<string, Task> tasks;
    public:

    void get_trace(){
        ifstream traces("tracefile_4KB_4GB_4GB.txt");
        string t;
    
        while (getline(traces, t)) { 
            process_trace(t);
        } 
        traces.close();
    }

    void process_trace(string t){
    	cout<<"Trace - "<<t<<endl;
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
        /*for (char ch : size) {
        if (std::isdigit(ch)) {
            size_number += ch;
        } else {
            break;  
        }
        //cout<<"Number of characters in size string = "<<count_character<<endl;
    }*/

        if(tasks.find(task_number)==tasks.end()){
        cout<<"added to tasks"<<endl;
        tasks[task_number] = Task(task_number);
        }
        //cout<<"size = "<<allocation_size+1<<endl;
        performance.map_check_physical_memory_allocated(tasks[task_number],logical_address,allocation_size);
        performance.singlePT_check_physical_memory_allocated(tasks[task_number],logical_address,allocation_size);
        //tasks[task_number].map_add(logical_address,stoi(size_number));
        //tasks[task_number].single_level_add(logical_address, stoi(size_number));
    }
	
	void display_task_details_map()
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
	
	void display_task_details_spt()
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
	
	void display_task_details_dpt()
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
};


int main() {
    IO ioManager;
    ioManager.get_trace();
    cout<<endl<<"Map Implementation"<<endl;
    ioManager.display_task_details_map();
    cout<<endl<<"Single Level Page Table Implementation"<<endl;
    ioManager.display_task_details_spt();
    
    //cout<<endl<<count;
    return 0;
}
