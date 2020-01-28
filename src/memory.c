#include "kernel/memory.h"
#include "libc/stdio.h"



// constructs a 64-bit integer from two 32-bit integers
#define cat64(h,l) (((uint64_t)h << 32) |(uint64_t)l)

// number of blocks of memory to reserver from the memory map
#define MEM_BLOCKS 10

// number of bytes in a page frame
#define FRAME_LEN 4096

// number of entries in a page table
#define PT_SIZE 1024

// number of tables in a page directory
#define PD_SIZE 1024


typedef multiboot_memory_map_t k_mmap;

typedef struct k_mem {
	uint64_t addr;
	uint64_t len;
}k_mem;

// memory available for dynamic allocation
k_mem mem[MEM_BLOCKS];

// number of memory blocks available
size_t mem_count = 0;

// number of pages in the second page table
size_t p2_count = 0;

// number of pages allocated from the second page table
size_t p2_res = 0;

// the primary page directory
uint32_t page_directory[1024] __attribute__((aligned(4096)));

// the first page table
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

// the second page table
uint32_t second_page_table[1024] __attribute__((aligned(4096)));


extern void load_page_directory(uint32_t*);
extern void enable_paging();

void k_memory_init(multiboot_info_t* mbi)
{
	uint64_t base = 0x400000;   // 1 MiB
	uint64_t length = 0x400000; // 4 MiB
	k_mmap* mmm;
	uint64_t a;
	uint64_t l;
	uint32_t t;
	uint64_t total;
	uint64_t res;
	uint64_t pos;
	size_t i, j;

	// First, we identity map the first 4 Mib of physical memory and
	// enable paging.

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
		// attributes: supervisor level, read/write, present.
		first_page_table[i] = (i * 0x1000) | 3;
	}

	// Insert the first page table into the page directory.
	page_directory[0] = ((uint32_t)first_page_table) | 3;

	// Put the address of the page directory in the CR3 register.
	load_page_directory(page_directory);

	// Enable paging.
	enable_paging();



	// Next, we try to acquire at least 4 MiB of memory above the
	// physical address 0x400000. We get this memory from the
	// multiboot memory map prepared by the bootloader.
	for (mmm = (k_mmap*)mbi->mmap_addr;
		(uint32_t)mmm < (mbi->mmap_addr + mbi->mmap_length);
		mmm = (k_mmap*)((uint32_t)mmm + mmm->size + sizeof(mmm->size)))
	{
		a = cat64(mmm->addr_hi, mmm->addr_lo);
		l = cat64(mmm->len_hi, mmm->len_lo);
		t = mmm->type;

		if (t == MULTIBOOT_MEMORY_AVAILABLE)
		{
			if (mem_count < MEM_BLOCKS && (a <= base && l > base - a))
			{
				// address is below the base, but the region
				// extends past the base.
				mem[mem_count].addr = base;
				mem[mem_count].len = l - (base - a);
				mem_count++;
			}
			else if (mem_count < MEM_BLOCKS && (a > base))
			{
				// address is above the base, and the region
				// has an arbitrary length.
				mem[mem_count].addr = a;
				mem[mem_count].len = l;
				mem_count++;
			}
		}
	}

	// Calculate how much memory we reserved.
	for (i = 0, total = 0; i < mem_count; i++)
		total += mem[i].len;

	if (total < length)
		fprintf(stddbg, "failed to allocate 4 MiB of memory\n");
	else
		fprintf(stddbg, "reserved %llu bytes of memory\n", total);

	// Fill the second page table with entries that map memory
	// above physical address 0x400000.
	// attributes: supervisor level, read/write, present.
	for (i = 0, j = 0, res = 0, pos = 0;
		i < 1024 && res < 0x400000 && res < total && j < MEM_BLOCKS;)
	{
		if (pos < mem[j].len && mem[j].len - pos >= FRAME_LEN)
		{
			second_page_table[i] = ((mem[j].addr + pos) ^ 0xFFF) | 3;
			pos += FRAME_LEN;
			res += FRAME_LEN;
			i++;
			p2_count++;
		}
		else
		{
			pos = 0;
			j++;
		}
	}

	// Insert the second page table into the page directory.
	page_directory[1] = ((uint32_t)second_page_table) | 3;
}



void* k_alloc()
{
	uint32_t v;   // virtual address
	uint32_t di;  // directory index (which table use)
	uint32_t ti;  // table index (which page to use)

	// For now, all page allocations will come from the
	// second page table in the page directory.
	di = 1;

	if (p2_res < p2_count && p2_res < PT_SIZE)
	{
		// Here is where we construct a virtual address.
		ti = (uint32_t)p2_res;

		// The 10 most significant bits are the index of the
		// page table within the page directory.
		v = di << 22;

		// The next 10 bits are the index of the page in the
		// page table.
		v |= (ti << 12);

		// The remaining 12 bits are the offset. These should
		// be 0 since we're allocating an entire page.

		// Increment the page reservation counter
		p2_res++;

		return (void*)v;
	}

	// We could not find an available page.
	return NULL;
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

void fprint_mmap(FILE* stream, multiboot_info_t* mbi)
{
	k_mmap* mmm;

	fprintf(stream, "+--------------------------------------------------+\n");
	fprintf(stream, "| Memory Map (address: %-#8X, length: %-#8X) |\n", mbi->mmap_addr, mbi->mmap_length);
	fprintf(stream, "+--------------------------------------------------+\n");
	fprintf(stream, "| address           length            type         |\n");
	fprintf(stream, "+--------------------------------------------------+\n");
	
	// 1MiB = 0x100000

	for (mmm = (k_mmap*)mbi->mmap_addr;
		(uint32_t)mmm < (mbi->mmap_addr + mbi->mmap_length);
		mmm = (k_mmap*)((uint32_t)mmm + mmm->size + sizeof(mmm->size)))
	{
		fprintf(stream, "| %-#16llX  %-#16llX  %-4d         |\n",
			cat64(mmm->addr_hi, mmm->addr_lo),
			cat64(mmm->len_hi, mmm->len_lo),
			mmm->type);
	}

	fprintf(stream, "+--------------------------------------------------+\n");
}

