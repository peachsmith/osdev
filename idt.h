#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_entry {
	uint16_t offset_low;  // offset bits 0..15
	uint16_t selector;    // a code segment selector in GDT or LDT
	uint8_t zero;         // unused, set to 0
	uint8_t type;         // type and attributes, see below
	uint16_t offset_high; // offset bits 16..31
}__attribute__((packed));

struct idt_handle
{
	uint16_t limit_size;
	struct idt_entry* base_addr;
}__attribute__((packed));

extern struct idt_entry idt_entries[256];
extern struct idt_handle idt_first;

extern void init_idt();

#endif
