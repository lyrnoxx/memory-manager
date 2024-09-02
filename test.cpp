#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <sstream>
#include <vector>
#include <iomanip>
#include "config.h"
#include <pthread.h>
using namespace std;

class Test{
	size_t page_size, total_tasks;
    mt19937 gen;
    ofstream file;
    pthread_mutex_t mutex;

	void generate_task_traces(size_t task_id){
		vector<pair<string, size_t>> sections ={{"text", TEXT}, {"data", DATA}, {"stack",STACK},{"shared", SHARED}, {"heap",HEAP}};
        uniform_int_distribution<> size_dist(1, 64); 
        for(const auto &section:sections){
            size_t logical_address = generate_random_address(section.second);
            size_t size = size_dist(gen);
            stringstream ss;
            ss<<"T"<<task_id<<":0x"<<setw(8)<<setfill('0')<<uppercase<<hex<<logical_address<<":"<<dec<<size<<"KB"<<endl;
            pthread_mutex_lock(&mutex);
            file<<ss.str();
            pthread_mutex_unlock(&mutex);
        }
	}

    size_t generate_random_address(size_t address){
        uniform_int_distribution<> dis(0,100);
        size_t offset = dis(gen)*page_size;
        return align_page_size(address+offset);
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

	size_t align_page_size(size_t address) const{
        return (address / page_size) * page_size;
    }

    static void *threading(void *arg){
        Test *test = static_cast<Test*>(arg);
        size_t task_id = test->total_tasks--;
        test->generate_task_traces(task_id);
        pthread_exit(nullptr);
    }

	public:
	Test(size_t page_size, size_t total_tasks, const string &filename): page_size(page_size*1024), 
			total_tasks(total_tasks), gen(random_device{}()), file(filename), mutex(PTHREAD_MUTEX_INITIALIZER){}

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

	void generate_file(){
        vector<pthread_t> threads(total_tasks);
        for(int i=0;i<total_tasks;i++){
            if (pthread_create(&threads[i], nullptr, threading, this) != 0) {
                cerr << "Error creating thread for task " << i + 1 << endl;
                exit(EXIT_FAILURE);
            }
        }
        for(int i=0;i<total_tasks;i++){
            pthread_join(threads[i], nullptr);
        }
        file.close();
        cout<<"traces genarated succesfully";
	}
};

int main(){

	Test test(2, 10, "traces.txt");
	test.generate_file();
}
