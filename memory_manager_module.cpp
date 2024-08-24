#include <iostream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

// all of the below variables will be defined in the config.h file.
/*extern int page_size;
extern int physical_memory_size;
extern int virutal_memory_size;
extern int address_size;
*/

static int page_size = 1;
static int physical_memory_size = 32;
static int virutal_memory_size = 32;
static int address_size = 32;

static int physical_page_number = 0;

class Memory_Manager
{
	public:
		long long int total_memory;
		int total_pages;
		int memory_allocated;
		int pages_allocated;
	
	Memory_Manager()
	{
		total_memory = pow(2,physical_memory_size);
		total_pages = pow(2,physical_memory_size)/pow(2,log2(page_size)+10);
		memory_allocated = 0;
		pages_allocated = 0;
	}
	
	void update_memory_allocated(int pages);
	void update_pages_allocated(int pages);
	void print_allocator_status();
	//vector<string> remove_offset(string logical_address);
	void add_offset();
	int hex_to_dec(string &logical_address);
	string dec_to_bin(int d_logical_address);
	int bin_to_dec(string page_number);
	string dec_to_hex(int page_number);
	void increment_physical_page_number();
	string increment_virtual_page_address(string address);
	string padding(string hex_address);
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

void Memory_Manager::print_allocator_status()
{
	cout<<"Total Memory Remaining = "<<total_memory - memory_allocated<<" bytes"<<endl;
	cout<<"Total Pages Remaining = "<<total_pages - pages_allocated<<"pages"<<endl;
	cout<<"Memory Allocated = "<<memory_allocated<<endl;
	cout<<"Pages Allocated = "<<pages_allocated<<endl;
	cout<<endl;
}

int Memory_Manager::hex_to_dec(string &logical_address)
{
	int decimal_value = 0;
	decimal_value = stoi(logical_address,nullptr,16);
	/*string hexa_array[6] = {"A","B","C","D","E","F"};
	for(int i=2;i<logical_address.length();i++)
	{
		if(logical_address[i] == hexa_array[0][0])
		{
			decimal_value += pow(16,10);
		}
		else if(logical_address[i] == hexa_array[1][0])
		{
			decimal_value += pow(16,11);
		}
		else if(logical_address[i] == hexa_array[2][0])
		{
			decimal_value += pow(16,12);
		}
		else if(logical_address[i] == hexa_array[3][0])
		{
			decimal_value += pow(16,13);
		}
		else if(logical_address[i] == hexa_array[4][0])
		{
			decimal_value += pow(16,14);
		}
		else if(logical_address[i] == hexa_array[5][0])
		{
			decimal_value += pow(16,15);
		}
		/*else if(string[i] == "x")
		{
			continue;
		}
		else
		{	
			decimal_value += pow(16,stoi(logical_address[i], nullptr));
		}*/
	//}
	return decimal_value;
}

string Memory_Manager::dec_to_bin(int d_logical_address)
{
	string address_bits = "";
	while(d_logical_address != 0)
	{
		int rem = d_logical_address%2;
		address_bits = to_string(rem) + address_bits;
		d_logical_address /= 2;
	}
	
	if(address_bits.length() < address_size)
	{
		int padding_length = address_size - address_bits.length();
		for(int i=0;i<padding_length;i++)
		{
			address_bits = "0" + address_bits;
		}
	}
	//cout<<"Length of the address bits = "<<address_bits.length()<<endl;
	return address_bits;
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
			hex_addr = to_string(rem) + hex_addr; //Wrong concatenation.
		}
		current_address /=16;
	}
	return hex_addr;
}

/*vector<string> Memory_Manager::remove_offset(string logical_address)
{
	vector<string> page_number_and_offset;
	int d_logical_address = hex_to_dec(logical_address);
	string address_bits = dec_to_bin(d_logical_address);
	int offset_size = log2(page_size) + 10;
	int address_length = address_bits.length();
	string offset = "";
	/*for(int i=(address_legnth - 1); i<=(address_length - offset_size); i--)
	{
		offset = address_bits + offset;
	}
	return offset;
}*/

void Memory_Manager::increment_physical_page_number()
{
	physical_page_number += 1;
}

int Memory_Manager::bin_to_dec(string page_number)
{
	int length = page_number.length();
	int d_page_number = 0;
	string one = "1";
	for(int i=0; i<length;i++)
	{
		if(page_number[i] == one[0])
		{
			int power = length-i-1;
			d_page_number += pow(2,power);
		}
	}
	return d_page_number;
}

string Memory_Manager::increment_virtual_page_address(string address)
{
	//vector<string> page_num_offset = remove_offset(address);
	int d_logical_address = hex_to_dec(address);
	d_logical_address += (page_size*1024);
	string new_address = dec_to_hex(d_logical_address);
	string logical_address = new_address;
	for(int i=0;i<(8-new_address.length()); i++)
	{
		logical_address = "0" + new_address;
	}
	logical_address = "0x"+logical_address;
	return logical_address;
	//string virtual_page_number = page_num_offset[1];
	//int d_virtual_page_number = bin_to_dec(virtual_page_number);
	//d_virtual_page_number += page_size
	//get page_number and offset from the page_num_offset vector.
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
	/*int offset_bits = log2(page_size) + 10;
	int length = bin_logical_address.length();
	string offset = bin_logical_address.substr(length-offset_bits,offset_bits);
	string bin_page_number = bin_physical_address.substr(0,length-offset_bits);
	bin_page_number += offset;
	if(bin_page_number.length() < address_size)
	{
		for(int i=0; i<(address_size-bin_page_number.length()); i++)
		{
			bin_page_number = "0" + bin_page_number;
		}
	}
	int d_physical_address = bin_to_dec(bin_page_number);
	string hexa_physical_address = dec_to_hex(d_physical_address);
	int hex_length = hexa_physical_address.length();
	if(hex_length < 8)
	{
		for(int i=0;i<8-hex_length;i++)
		{
			hexa_physical_address = "0"+hexa_physical_address;
		}
	}
	hexa_physical_address = "0x"+hexa_physical_address;
	return hexa_physical_address;
	*/
	
}

string Memory_Manager::get_next_virtual_address(string virtual_address)
{
	//virtual_address is hexadecimal.
        long int dec_virtual_address = hex_to_dec(virtual_address);
        //cout<<"dec virtual address = "<<dec_virtual_address<<endl;
	//string bin_virtual_address = dec_to_bin(dec_virtual_address);
	
	dec_virtual_address += page_size*1024;
	//cout<<"new dec virtual address = "<<dec_virtual_address<<endl;
	string new_virtual_address = dec_to_hex(dec_virtual_address);
	//cout<<"new virtual address = "<<new_virtual_address<<endl;
	new_virtual_address = padding(new_virtual_address);
	//cout<<"padded new virtual address = "<<new_virtual_address<<endl;
	//cout<<endl;
	return new_virtual_address;
}

string Memory_Manager::get_physical_address()
{
	long int dec_physical_address = physical_page_number*page_size*1024;
	//cout<<"decimal physical address = "<<dec_physical_address<<endl;
	string hex_physical_address = dec_to_hex(dec_physical_address);
	//cout<<"hex physical address = "<<hex_physical_address<<endl;
	string physical_address = padding(hex_physical_address);
	increment_physical_page_number();
	return physical_address;
}

vector<vector<string>> Memory_Manager::assign_memory(string virtual_page_address, int size)
{
	int num_pages;
	if(size%page_size == 0)
	{
		num_pages = size/page_size;
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
	
	for(int j=0;j<vp_address[0].size();j++)
	{
		cout<<"Logical Address of "<<j+1<<" is = "<<vp_address[0][j]<<endl;
		cout<<"Physical Address of "<<j+1<<" is = "<<vp_address[1][j]<<endl;
		cout<<endl;
	}
	
	allocator.print_allocator_status();
	
	return 0;	
}
