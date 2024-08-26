#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <sstream>
#include <vector>
using namespace std;

class Test{
	size_t page_size;
	vector<string> tasks ={"text", "data", "stack", "heap"};

	string generate_hex_address(size_t address){
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
	}

	size_t align_page_size(size_t address) {
        return (address / page_size) * page_size;
    }

	public:
	Test(size_t size): page_size(size*1024){}

	vector<string> generate_traces(int total_tasks, int total_traces){
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
	}

	void generate_file(const vector<string>& traces, const string& filename){
		ofstream file(filename);
		if(file.is_open()){
			for(const auto& trace: traces){
				file<<trace<<endl;
			}
			file.close();
			cout<<"traces generated and saved to "<<filename<<endl;
		}else{
			cerr<<"unable to create or open file: "<< filename<<endl;
		}
	}
};

int main(){
	size_t page_size = 4;
	int total_tasks = 5;
	int total_traces = 100;

	Test test(page_size);

	vector<string> traces = test.generate_traces(total_tasks,total_traces);
	test.generate_file(traces, "traces.txt");
}