#include <iostream>
#include <string>
#include <map>

extern int page_size;
extern int logical_memory_size;
extern int physical_memory_size;
extern int address_size; //number of bits for physical address.
extern int page_entry_size; //number of bytes required for each page entry.

static int map_memory_hits = 0;
static int map_memory_misses = 0;

static int single_PT_memory_hits = 0;
static int single_PT_memory_misses = 0;


class Performance : public Memory_Manager
{
	public:
		Memory_Manager memory;
	
	int calculate_page_table_size();
	long int hex_to_dec(string logical_address);
	string get_next_virtual_address();
	void map_check_physical_memory_allocated(Task* task, string logical_address, int size);
	void singlePT_check_physical_memory_allocated(Task* task, string logical_address, int size);
}
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
	//virtual_address is hexadecimal.
        /*long int dec_virtual_address = hex_to_dec(virtual_address);
	dec_virtual_address += page_size*1024;
	string new_virtual_address = dec_to_hex(dec_virtual_address);
	new_virtual_address = padding(new_virtual_address);
	return new_virtual_address;*/
	Memory_Manager m;
	string new_virtual_address = m.get_next_virtual_address(virtual_address);
	return new_virtual_address;
}

//Task* task should send a pointer to a list of tasks, to access the data.
void Performance::map_check_physical_memory_allocated(Task* task, string task_name, string logical_address, int size)
{
	int pages, allocation_needed;
	string start_allocation = "";
	allocation_needed = 0;
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
			memory_hits += 1;
			logical_address = get_next_virtual_address(logical_address);
		}
		else
		{
			memory_misses += 1;
			allocation_needed += 1;
			if(start_allocation == "")
			{
				start_allocation = logical_address;
			}
		}
	}
	
	if(start_allocation != "")
	{
		assign_memory(start_allocation, allocation_needed);//inherited function should be written in the class above.
	}
}

void singlePT_check_physical_memory_allocated(Task* task,string task_name string logical_address, int size)
{
	long int dec_logical_address = hex_to_dec(logical_address);
	long int page_number = dec_logical_address/(page_size*1024);
	task
}
//call the update_free_memory() function of the Memory_Manager module.
//call the get_free_memory() function of the Memory_Manager Module 
//to get free memory left.

int main()
{
	//either use inheritance or we can create an instance of Memory_Manager when 
	//needed and use it to do the required operations.
	Performance p1;
	//I/O module can create a performance object and then send entire task list to 
	//map_check_physical_memory_allocated() with other required arguments.
	//One issue is that we will have to pass all 10 task objects,
	//containing big amounts of data everytime a new task instance comes in.
	//Can use pointer to pass the array containing the task objects.
	
	//OR we can use inheritance and make I/O module the child of Performance and be 
	//able to access the functions of Performance without really making an object.
	
	//Finally to measure execution time, we will enclose the function call within 
	//time value.
	return 0;
}
