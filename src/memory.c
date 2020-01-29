#include "kernel/memory.h"
#include "libc/stdio.h"


// converts a pointer to an unsigned integer
#define ptou(x) ((uint32_t)(x))


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

void validate_rsdp(char* rsdp)
{
	size_t i;
	unsigned char check;    // checksum
	unsigned char oemid[6]; // OEIM string
	unsigned char rev;      // revision number
	unsigned char rsdt[4];  // 32-bit physical address of RSDT

	unsigned char len[4];   // size of the table in bytes
	unsigned char xsdt[8];  // 64-bit physical address of XSDT
	unsigned char xcheck;   // extended checksum

	// The end of the RSDP structure contains 3 reserved bytes.
	unsigned char reserved[3];

	// according to the ACPI spec,
	// all multi-byte numeric values are in little endian

	check = *(rsdp + 8);

	oemid[0] = *(rsdp + 9);
	oemid[1] = *(rsdp + 10);
	oemid[2] = *(rsdp + 11);
	oemid[3] = *(rsdp + 12);
	oemid[4] = *(rsdp + 13);
	oemid[5] = *(rsdp + 14);

	rev = *(rsdp + 15);

	rsdt[0] = *(rsdp + 16);
	rsdt[1] = *(rsdp + 17);
	rsdt[2] = *(rsdp + 18);
	rsdt[3] = *(rsdp + 19);

	if ( rev >= 2)
	{
		len[0] = *(rsdp + 20);
		len[1] = *(rsdp + 21);
		len[2] = *(rsdp + 22);
		len[3] = *(rsdp + 23);

		xsdt[0] = *(rsdp + 24);
		xsdt[1] = *(rsdp + 25);
		xsdt[2] = *(rsdp + 26);
		xsdt[3] = *(rsdp + 27);
		xsdt[4] = *(rsdp + 28);
		xsdt[5] = *(rsdp + 29);
		xsdt[6] = *(rsdp + 30);
		xsdt[7] = *(rsdp + 31);

		xcheck = *(rsdp + 32);
	}

	unsigned char valid = check
		+ (unsigned char)(rsdp[0])
		+ (unsigned char)(rsdp[1])
		+ (unsigned char)(rsdp[2])
		+ (unsigned char)(rsdp[3])
		+ (unsigned char)(rsdp[4])
		+ (unsigned char)(rsdp[5])
		+ (unsigned char)(rsdp[6])
		+ (unsigned char)(rsdp[7])
		+ oemid[0]
		+ oemid[1]
		+ oemid[2]
		+ oemid[3]
		+ oemid[4]
		+ oemid[5]
		+ rev
		+ rsdt[0]
		+ rsdt[1]
		+ rsdt[2]
		+ rsdt[3];

	fprintf(stddbg, "checksum verification: %d\n", valid);

	fprintf(stddbg, "OEMID: ");
	for (i = 0; i < 6; i++)
		fputc(oemid[i], stddbg);
	
	fputc('\n', stddbg);
}

void check_ebda()
{
	uint32_t ebda_addr = *((uint16_t*)0x040E);
	char* ebda = (char*)(ebda_addr);

	while (ptou(ebda) < 0xA0000)
	{
		if (*ebda == 'R' && ptou(ebda) < 0xA0000 - 33)
		{
			if (ebda[0] == 'R'
				&& ebda[1] == 'S'
				&& ebda[2] == 'D'
				&& ebda[3] == ' '
				&& ebda[4] == 'P'
				&& ebda[5] == 'T'
				&& ebda[6] == 'R'
				&& ebda[7] == ' ')
			{
				fprintf(stddbg, "found RSD PTR in EBDA\n");
				validate_rsdp(ebda);
			}
		}

		ebda++;
	}
}

void check_e0000()
{
	char* str = (char*)0x000E0000;

	while ((uint32_t)str < 0x000FFFFF)
	{
		if (*str == 'R' && ptou(str) < 0x000FFFFF - 33)
		{
			if (str[0] == 'R'
				&& str[1] == 'S'
				&& str[2] == 'D'
				&& str[3] == ' '
				&& str[4] == 'P'
				&& str[5] == 'T'
				&& str[6] == 'R'
				&& str[7] == ' ')
			{
				fprintf(stddbg, "found RSD PTR in 0xE0000 to 0xF0000\n");
				validate_rsdp(str);
			}
		}

		str++;
	}
}

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


	// search for the RSDP
	check_ebda();
	check_e0000();


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
		if (pos < mem[j].len
			&& mem[j].len - pos >= FRAME_LEN
			&& mem[j].addr + pos < 0xFFFFFFFF - FRAME_LEN)
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



void* k_palloc()
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

