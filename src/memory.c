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

uint32_t dir_i = 0;   // directory index [0, 1023]
uint32_t tab_i = 1; // table index     [0, 1023]
uint32_t offset = 0;  // frame offset    [0, 4095]

/**
 * The goal of this function is to construct a valid pointer to virtual memory.
 * There is currently only one page table with all of its 1024 entries
 * populated with identity mapping.
 * So the physical addresses 0x00 - 0x3FFFFF are mapped to the virtual
 * addresses 0x00 - 0x3FFFFF.
 */
void* build_pointer(size_t n)
{	
	uint32_t v_addr; // a virtual address

	// shift the 10 bit directory index to the left by 22	
	v_addr = dir_i << 22;
	
	// OR the address with the 10-bit table index shifted to the left by 12
	v_addr |= (tab_i << 12);
	
	// for now, we're only allowing allocation within one frame
	// and not worrying about freeing memory
	if (offset + n < 0xFFF)
	{
		v_addr |= offset;
		offset += n;
		return (void*)v_addr;
	}
	
	return NULL;
}
