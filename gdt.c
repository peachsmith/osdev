#include "gdt.h"

// Segments
#define NULL_SEGMENT 0
#define CODE_SEGMENT 1
#define DATA_SEGMENT 2

/**
 * The GDT.
 */
gdt_entry gdt[3];

/**
 * The address of the first GDT entry.
 */
gdt_handle gdt_first;

/**
 * Executes the lgdt instruction to load the GDT.
 */
extern void load_gdt(gdt_entry*);

static void set_gdt_entry(uint8_t segment,
	uint16_t limit,
	uint32_t base,
	uint8_t access,
	uint8_t granularity)
{
	gdt[segment].segment_limit = limit;
	gdt[segment].base_low = (base & 0xffff);
	gdt[segment].base_mid = (base & 0x00FF0000) >> 16;
	gdt[segment].access = access;
	gdt[segment].granularity = granularity;
	gdt[segment].base_high = (base & 0xFF000000) >> 24;
}

void init_gdt()
{
	set_gdt_entry(NULL_SEGMENT, 0, 0, 0, 0);

	// access: 10011010
	// granularity: 11001111
	set_gdt_entry(CODE_SEGMENT, 0xFFFF, 0, 0x9A, 0xCF);

	// access: 10010010
	// granularity: 11001111
	set_gdt_entry(DATA_SEGMENT, 0xFFFF, 0, 0x92, 0xCF);


	// Build the handle to the first GDT entry.
	gdt_first.limit_size = sizeof(gdt) - 1;
	gdt_first.base_addr = (struct gdt_entry*)(&gdt);

	// Load the GDT.
	load_gdt((gdt_entry*)(&gdt_first));
}

