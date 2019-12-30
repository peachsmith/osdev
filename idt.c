#include "idt.h"
#include "port.h"

struct idt_entry idt_entries[256];
struct idt_handle idt_first;

extern void load_idt();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void init_idt()
{
	uintptr_t irq0_address = (uintptr_t)irq0;
	uintptr_t irq1_address = (uintptr_t)irq1;
	uintptr_t irq2_address = (uintptr_t)irq2;
	uintptr_t irq3_address = (uintptr_t)irq3;
	uintptr_t irq4_address = (uintptr_t)irq4;
	uintptr_t irq5_address = (uintptr_t)irq5;
	uintptr_t irq6_address = (uintptr_t)irq6;
	uintptr_t irq7_address = (uintptr_t)irq7;
	uintptr_t irq8_address = (uintptr_t)irq8;
	uintptr_t irq9_address = (uintptr_t)irq9;
	uintptr_t irq10_address = (uintptr_t)irq10;
	uintptr_t irq11_address = (uintptr_t)irq11;
	uintptr_t irq12_address = (uintptr_t)irq12;
	uintptr_t irq13_address = (uintptr_t)irq13;
	uintptr_t irq14_address = (uintptr_t)irq14;
	uintptr_t irq15_address = (uintptr_t)irq15;

	// Remap the PIC
	k_outb(0x20, 0x11);
	k_outb(0xA0, 0x11);
	k_outb(0x21, 0x20);
	k_outb(0xA1, 0x28);
	k_outb(0x21, 0x04);
	k_outb(0xA1, 0x02);
	k_outb(0x21, 0x01);
	k_outb(0xA1, 0x01);
	k_outb(0x21, 0x0);
	k_outb(0xA1, 0x0);

	// TODO: might need to change the selector to be 1,
	// since that's the GDT code segment

	idt_entries[32].offset_low = irq0_address & 0xFFFF;
	idt_entries[32].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[32].zero = 0;
	idt_entries[32].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[32].offset_high = (irq0_address & 0xFFFF0000) >> 16;

	idt_entries[33].offset_low = irq1_address & 0xFFFF;
	idt_entries[33].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[33].zero = 0;
	idt_entries[33].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[33].offset_high = (irq1_address & 0xFFFF0000) >> 16;

	idt_entries[34].offset_low = irq2_address & 0xFFFF;
	idt_entries[34].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[34].zero = 0;
	idt_entries[34].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[34].offset_high = (irq2_address & 0xFFFF0000) >> 16;

	idt_entries[35].offset_low = irq3_address & 0xFFFF;
	idt_entries[35].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[35].zero = 0;
	idt_entries[35].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[35].offset_high = (irq3_address & 0xFFFF0000) >> 16;

	idt_entries[36].offset_low = irq4_address & 0xFFFF;
	idt_entries[36].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[36].zero = 0;
	idt_entries[36].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[36].offset_high = (irq4_address & 0xFFFF0000) >> 16;

	idt_entries[37].offset_low = irq5_address & 0xFFFF;
	idt_entries[37].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[37].zero = 0;
	idt_entries[37].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[37].offset_high = (irq5_address & 0xFFFF0000) >> 16;

	idt_entries[38].offset_low = irq6_address & 0xFFFF;
	idt_entries[38].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[38].zero = 0;
	idt_entries[38].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[38].offset_high = (irq6_address & 0xFFFF0000) >> 16;

	idt_entries[39].offset_low = irq7_address & 0xFFFF;
	idt_entries[39].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[39].zero = 0;
	idt_entries[39].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[39].offset_high = (irq7_address & 0xFFFF0000) >> 16;

	idt_entries[40].offset_low = irq8_address & 0xFFFF;
	idt_entries[40].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[40].zero = 0;
	idt_entries[40].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[40].offset_high = (irq8_address & 0xFFFF0000) >> 16;

	idt_entries[41].offset_low = irq9_address & 0xFFFF;
	idt_entries[41].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[41].zero = 0;
	idt_entries[41].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[41].offset_high = (irq9_address & 0xFFFF0000) >> 16;

	idt_entries[42].offset_low = irq10_address & 0xFFFF;
	idt_entries[42].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[42].zero = 0;
	idt_entries[42].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[42].offset_high = (irq10_address & 0xFFFF0000) >> 16;

	idt_entries[43].offset_low = irq11_address & 0xFFFF;
	idt_entries[43].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[43].zero = 0;
	idt_entries[43].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[43].offset_high = (irq11_address & 0xFFFF0000) >> 16;

	idt_entries[44].offset_low = irq12_address & 0xFFFF;
	idt_entries[44].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[44].zero = 0;
	idt_entries[44].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[44].offset_high = (irq12_address & 0xFFFF0000) >> 16;

	idt_entries[45].offset_low = irq13_address & 0xFFFF;
	idt_entries[45].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[45].zero = 0;
	idt_entries[45].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[45].offset_high = (irq13_address & 0xFFFF0000) >> 16;

	idt_entries[46].offset_low = irq14_address & 0xFFFF;
	idt_entries[46].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[46].zero = 0;
	idt_entries[46].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[46].offset_high = (irq14_address & 0xFFFF0000) >> 16;

	idt_entries[47].offset_low = irq15_address & 0xFFFF;
	idt_entries[47].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idt_entries[47].zero = 0;
	idt_entries[47].type = 0x8E; /* INTERRUPT_GATE */
	idt_entries[47].offset_high = (irq15_address & 0xFFFF0000) >> 16;

	idt_first.limit_size = sizeof(idt_entries) * 256 - 1;
	idt_first.base_addr = (struct idt_entry*) & idt_entries;

	load_idt((struct gdt_entry*) & idt_first);
}
