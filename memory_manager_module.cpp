#include <iostream>
#include <string>
#include <cmath>
#include <vector>
//#include "config.h"

using namespace std;

// all of the below variables will be defined in the config.h file.
/*
extern int page_size;
extern int physical_memory_size;
extern int logical_memory_size;
extern int address_size;
extern int page_entry_size;
*/

//comment the below once all the modules are linked.
static int page_size_mm = 4;
static int physical_memory_size_mm = 32;
static int virtual_memory_size_mm = 32;
static int address_size_mm = 32;
static int page_entry_size_mm = 4;


//We are starting the physical page from 0th position.
static int physical_page_number = 0;

/*
Memory Manager class will have only once instance and it will assign physical memory 
addresses to a request for pages.

Depending on the number of pages, this module will also generate logical addresses for 
the map implementation part.

Input will be provided from the Task module in the form of a string(logical address) and
int(size of task instance).

This module will return a vector of vectors of strings. A vector each of logical addresses
and physical addresses will be placed within another vector and returned.
*/ 
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
		total_memory = pow(2,physical_memory_size_mm);
		total_pages = pow(2,physical_memory_size_mm)/pow(2,log2(page_size_mm)+10);
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
	memory_allocated += pages * pow(2,log2(page_size_mm)+10);
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
	d_logical_address += (page_size_mm*1024);
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
	int hex_address_length = (address_size_mm/4); //need to get ceil but keeping it like this for now.
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
	dec_virtual_address += page_size_mm*1024;
	string new_virtual_address = dec_to_hex(dec_virtual_address);
	new_virtual_address = padding(new_virtual_address);
	return new_virtual_address;
}

string Memory_Manager::get_physical_address()
{
	long int dec_physical_address = physical_page_number*page_size_mm*1024;
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
	if(size%page_size_mm == 0)
	{
		num_pages = (size/page_size_mm);
	}
	else
	{
		num_pages = (size/page_size_mm) + 1;
	}
	
	//updating the attributes of the class Memory_Manager.
	update_memory_allocated(num_pages);
	update_pages_allocated(num_pages);
	
	//defining a vector for storing 
	vector<string> logical_addresses;
	vector<string> physical_addresses;
	//cout<<"num_pages = "<<num_pages<<endl;
	for(int i=0;i<num_pages;i++)
	{
		//cout<<"Iteration number = "<<i<<endl;
		//cout<<"Logical address being put into the vector is - "<<virtual_page_address<<endl;
		logical_addresses.push_back(virtual_page_address);
		string physical_address = get_physical_address();
		
		//cout<<"Physical address being put into the vector is - "<<physical_address<<endl;
		physical_addresses.push_back(physical_address);
		virtual_page_address = get_next_virtual_address(virtual_page_address);
		//cout<<"new Virtual address = "<<virtual_page_address<<endl;
	}
	vector<vector<string>> address;
	address.push_back(logical_addresses);
	address.push_back(physical_addresses);
	//cout<<"Exiting the Memory Manager Module"<<endl<<endl;
	return address;
}

/*
int main()
{
	Memory_Manager allocator;
	string task_address;
	int size;
	vector<vector<string>> vp_address;
	
	cin>>task_address;
	cin>>size;
	
	allocator.print_allocator_status();
	vp_address = allocator.assign_memory(task_address, size);
	
	allocator.print_allocator_status();
	
	return 0;	
}
*/
