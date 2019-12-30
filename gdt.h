#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt_entry
{
	uint16_t segment_limit;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t granularity; // high 4: flags, low 4: limit high
	uint8_t base_high;
}__attribute__((packed));

struct gdt_handle
{
	uint16_t limit_size;
	struct gdt_entry* base_addr;
}__attribute__((packed));

extern struct gdt_entry gdt_entries[3];
extern struct gdt_handle gdt_first;

extern void init_gdt();

#endif

