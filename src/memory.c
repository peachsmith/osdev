#include "kernel/memory.h"
#include "kernel/serial.h"

/**
 * The page directory
 */
uint32_t page_directory[1024] __attribute__((aligned(4096)));

/**
 * The first page table
 */
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

uint32_t last_page = 255;
uint32_t last_res = 0; // don't worry about frees for now

/**
 * 
 */
extern void load_page_directory(uint32_t*);
extern void enable_paging();

void init_paging()
{
	uint32_t i;

	for (i = 0; i < 1024; i++)
	{
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		page_directory[i] = 0x00000002;
	}

	// Fill all 1024 entries in the first paging table (4 MB)
	for (i = 0; i < 1024; i++)
	{
		// As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes.

		// attributes: supervisor level, read/write, present.
		first_page_table[i] = (i * 0x1000) | 3;
	}

	// Insert the first page table into the page directory.
	page_directory[0] = ((uint32_t)first_page_table) | 3;

	load_page_directory(page_directory);

	// Enable paging.
	enable_paging();
}

/**
 * The goal of this function is to construct a valid pointer to virtual memory.
 * There is currently only one page table with all of its 1024 entries
 * populated with identity mapping.
 * So the physical addresses 0x00 - 0x3FFFFF are mapped to the virtual
 * addresses 0x00 - 0x3FFFFF.
 */
void* build_pointer(size_t n)
{	
	// last_page is a number ranging from 255 to 1023
	// that represents the index of the first page table.
	uint32_t p = last_page << 12;
	
	// For now, only allow up to 4095 bytes to be allocated.
	// We're only concerned with constructing a valid pointer.
	if (last_res + n < 0xFFF)
	{
		p |= last_res;
		last_res += n;
		return (void*)p;
	}
	
	return NULL;
}
