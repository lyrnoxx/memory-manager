#include<iostream>
#include<fstream>
#include<map>
#include<unordered_map>
using namespace std;

class Task{
    string task_number, physical_address;
    unordered_map<string, string> page_table;
    public:
        Task() : task_number("") {}
        Task(string n): task_number(n){}
        void add(string logical_address, string size){
            if(page_table.find(logical_address)!=page_table.end()){
                cout<<endl<<"PAGE HIT!!"<<endl;
            }
            else{
                page_table[logical_address]="8347298347928";
            }
        /*cout << "Page table for Task " << task_number << ":" << endl;
        for (const auto &entry : page_table) {
            cout << entry.first << " " << entry.second << endl;
        }
        cout << endl;*/
        }
};

class IO{
    unordered_map<string, Task> tasks;
    public:
    void get_trace(){
        ifstream traces("tracefile_1KB_8GB_16GB.txt");
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
        tasks[task_number].add(logical_address,size);
    }

};


int main() {
    IO ioManager;
    ioManager.get_trace();

    return 0;
}