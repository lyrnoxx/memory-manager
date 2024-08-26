#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <sstream>
#include <vector>
#include "config.h"
using namespace std;

class Test{
	size_t page_size, total_tasks, traces_per_task;

	void generate_task_traces(ofstream& trace_file, size_t task_id){
		vector<pair<string, size_t>> sections ={{"text", TEXT}, {"data", DATA}, {"stack",STACK},{"shared", SHARED}, {"heap",HEAP}};
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> size_dist(1, 100);
        uniform_int_distribution<> section_dist(0, sections.size() - 1); 
		for (size_t j = 0; j < traces_per_task; ++j) {
            size_t section_index = section_dist(gen);
            const auto& section = sections[section_index];
            size_t base_address = section.second;
            size_t random_offset = (gen() % 100) * page_size; 
            size_t aligned_address = align_page_size(base_address + random_offset);
            size_t size_kb = size_dist(gen);

            stringstream ss;
            ss << "T" << task_id << ":0x" << uppercase << hex << aligned_address << ":" << size_kb << "KB";
            trace_file << ss.str() << endl;
        }
	}

	/*string generate_hex_address(size_t address){
		stringstream ss;
		ss<<"0x"<<uppercase<<hex<<address;
		return ss.str();
	}

	string generate_task_id(int total_tasks){
		return "T"+ to_string(rand()% total_tasks+1);
	}

	string generate_section(){
		return tasks[rand()% tasks.size()];
	}

	int generate_size( int min, int max){
		return (rand() % (max - min + 1) + min) * 1024;
	}*/

	size_t align_page_size(size_t address) {
        return (address / page_size) * page_size;
    }

	public:
	Test(size_t size, size_t total_tasks, size_t traces_per_task): page_size(size*1024), 
			total_tasks(total_tasks), traces_per_task(traces_per_task){}

	/*vector<string> generate_traces(int total_tasks, int total_traces){
		vector<string> traces;
		size_t address = 0x10000000;

		for(int i=0; i<total_traces;i++){
			string task_id = generate_task_id(total_tasks);
			size_t logical_address = align_page_size(address+(i*page_size)*(rand()%10+1));
			int size = generate_size(1,64);
			stringstream trace;
			trace<<task_id<<":"<<generate_hex_address(logical_address)<<":"<<size/1024<<"KB";
			traces.push_back(trace.str());
		}
		return traces;
	}*/

	void generate_file(const string& filename){
		ofstream file(filename);
		if(file.is_open()){
			for (size_t i = 1; i <= total_tasks; ++i) {
            generate_task_traces(file, i);
        }

			file.close();
			cout<<"traces generated and saved to "<<filename<<endl;
		}else{
			cerr<<"unable to create or open file: "<< filename<<endl;
		}
	}
};

int main(){

	Test test(4, 10, 100);
	test.generate_file("traces.txt");
}