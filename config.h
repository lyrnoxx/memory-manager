#define TEXT 0x00400000  
#define DATA 0x01000000  
#define STACK 0x08000000  
#define SHARED 0x10000000  
#define HEAP 0x20000000 



int page_size = 4;
int logical_memory_size = 32;
int physical_memory_size = 32;
int address_size = 32; //number of bits for physical address.
int page_entry_size = 4; //number of bytes required for each page entry.


