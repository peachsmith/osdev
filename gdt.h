#ifndef GDT_H
#define GDT_H

#include <stdint.h>

/**
 * A single GDT entry.
 */
struct gdt_entry
{
	uint16_t segment_limit;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t granularity; // high 4: flags, low 4: limit high
	uint8_t base_high;
}__attribute__((packed));

/**
 * A structure representing the address of a GDT.
 */
struct gdt_handle
{
	uint16_t limit_size;
	struct gdt_entry* base_addr;
}__attribute__((packed));

typedef struct gdt_entry gdt_entry;
typedef struct gdt_handle gdt_handle;

/**
 * The GDT.
 */
extern gdt_entry gdt[3];

/**
 * The address of the first GDT entry.
 */
extern gdt_handle gdt_first;

/**
 * Builds the GDT.
 */
extern void init_gdt();

#endif

