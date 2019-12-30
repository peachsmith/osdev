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
	void(*irq0_address)() = (void(*)())irq0;
	void(*irq1_address)() = (void(*)())irq1;
	void(*irq2_address)() = (void(*)())irq2;
	void(*irq3_address)() = (void(*)())irq3;
	void(*irq4_address)() = (void(*)())irq4;
	void(*irq5_address)() = (void(*)())irq5;
	void(*irq6_address)() = (void(*)())irq6;
	void(*irq7_address)() = (void(*)())irq7;
	void(*irq8_address)() = (void(*)())irq8;
	void(*irq9_address)() = (void(*)())irq9;
	void(*irq10_address)() = (void(*)())irq10;
	void(*irq11_address)() = (void(*)())irq11;
	void(*irq12_address)() = (void(*)())irq12;
	void(*irq13_address)() = (void(*)())irq13;
	void(*irq14_address)() = (void(*)())irq14;
	void(*irq15_address)() = (void(*)())irq15;
	struct idt_entry* idt_address;
	struct idt_entry* idt_ptr[2];

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
	idt_first.base_addr = (struct idt_entry*)&idt_entries;

	load_idt((struct gdt_entry*)&idt_first);
}
