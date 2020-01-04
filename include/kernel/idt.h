#ifndef JEP_K_IDT_H
#define JEP_K_IDT_H

#include <stdint.h>

/**
 * A single IDT entry.
 */
struct idt_entry {
	uint16_t offset_low;  // offset bits [0, 15]
	uint16_t selector;    // a code segment selector in GDT or LDT
	uint8_t zero;         // unused, set to 0
	uint8_t type;         // type and attributes
	uint16_t offset_high; // offset bits [16, 31]
}__attribute__((packed));

/**
 * A structure representing the address of an IDT.
 */
struct idt_handle
{
	uint16_t limit_size;
	struct idt_entry* base_addr;
}__attribute__((packed));

typedef struct idt_entry idt_entry;
typedef struct idt_handle idt_handle;

/**
 * The IDT.
 */
extern idt_entry idt[256];

/**
 * The address of the first IDT entry.
 */
extern idt_handle idt_first;

/**
 * Populates the IDT.
 */
extern void init_idt();

#endif
