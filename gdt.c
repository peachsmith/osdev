#include "gdt.h"

#define NULL_SEGMENT 0
#define CODE_SEGMENT 1
#define DATA_SEGMENT 2

struct gdt_entry gdt_entries[3];
struct gdt_handle gdt_first;

extern void load_gdt(struct gdt_entry*);

void init_gdt()
{
	// null segment
	gdt_entries[NULL_SEGMENT].segment_limit = 0;
	gdt_entries[NULL_SEGMENT].base_low = 0;
	gdt_entries[NULL_SEGMENT].base_mid = 0;
	gdt_entries[NULL_SEGMENT].access = 0;
	gdt_entries[NULL_SEGMENT].granularity = 0;
	gdt_entries[NULL_SEGMENT].base_high = 0;

	// code segment
	gdt_entries[CODE_SEGMENT].segment_limit = 0xFFFF;
	gdt_entries[CODE_SEGMENT].base_low = 0;
	gdt_entries[CODE_SEGMENT].base_mid = 0;
	gdt_entries[CODE_SEGMENT].access = 0x9A;
	gdt_entries[CODE_SEGMENT].granularity = 0xCF; // 11001111
	gdt_entries[CODE_SEGMENT].base_high = 0;

	// data segment
	gdt_entries[DATA_SEGMENT].segment_limit = 0xFFFF;
	gdt_entries[DATA_SEGMENT].base_low = 0;
	gdt_entries[DATA_SEGMENT].base_mid = 0;
	gdt_entries[DATA_SEGMENT].access = 0x92;
	gdt_entries[DATA_SEGMENT].granularity = 0xCF; // 11001111
	gdt_entries[DATA_SEGMENT].base_high = 0;

	gdt_first.limit_size = sizeof(gdt_entries) - 1;
	gdt_first.base_addr = (struct gdt_entry*)&gdt_entries;

	load_gdt((struct gdt_entry*)&gdt_first);
}

