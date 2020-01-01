#include "idt.h"
#include "port.h"

/**
 * Interrupt Service Routines
 */

/**
 * The IDT.
 */
idt_entry idt[256];

/**
 * The address of the first IDT entry.
 */
idt_handle idt_first;

/**
 * Executes the lidt instruction to load the IDT.
 */
extern void load_idt(idt_entry*);

// ISRs for exceptions and reserved interrupts
extern void isr_0();
extern void isr_1();
extern void isr_2();
extern void isr_3();
extern void isr_4();
extern void isr_5();
extern void isr_6();
extern void isr_7();
extern void isr_8();
extern void isr_9();
extern void isr_10();
extern void isr_11();
extern void isr_12();
extern void isr_13();
extern void isr_14();
extern void isr_15();
extern void isr_16();
extern void isr_17();
extern void isr_18();
extern void isr_19();
extern void isr_20();
extern void isr_21();
extern void isr_22();
extern void isr_23();
extern void isr_24();
extern void isr_25();
extern void isr_26();
extern void isr_27();
extern void isr_28();
extern void isr_29();
extern void isr_30();
extern void isr_31();

// IRQs for hardware interrupts
extern void irq_0();
extern void irq_1();
extern void irq_2();
extern void irq_3();
extern void irq_4();
extern void irq_5();
extern void irq_6();
extern void irq_7();
extern void irq_8();
extern void irq_9();
extern void irq_10();
extern void irq_11();
extern void irq_12();
extern void irq_13();
extern void irq_14();
extern void irq_15();

static void set_idt_entry(uint8_t n,
	uintptr_t addr,
	uint16_t sel,
	uint8_t type)
{
	idt[n].offset_low = addr & 0xFFFF;
	idt[n].selector = sel;
	idt[n].zero = 0;
	idt[n].type = type;
	idt[n].offset_high = (addr & 0xFFFF0000) >> 16;
}

void init_idt()
{
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

	// Register the ISR for interrupts 0 through 31.
	set_idt_entry(0, (uintptr_t)isr_0, 0x08, 0x8E);
	set_idt_entry(1, (uintptr_t)isr_1, 0x08, 0x8E);
	set_idt_entry(2, (uintptr_t)isr_2, 0x08, 0x8E);
	set_idt_entry(3, (uintptr_t)isr_3, 0x08, 0x8E);
	set_idt_entry(4, (uintptr_t)isr_4, 0x08, 0x8E);
	set_idt_entry(5, (uintptr_t)isr_5, 0x08, 0x8E);
	set_idt_entry(6, (uintptr_t)isr_6, 0x08, 0x8E);
	set_idt_entry(7, (uintptr_t)isr_7, 0x08, 0x8E);
	set_idt_entry(8, (uintptr_t)isr_8, 0x08, 0x8E);
	set_idt_entry(9, (uintptr_t)isr_9, 0x08, 0x8E);
	set_idt_entry(10, (uintptr_t)isr_10, 0x08, 0x8E);
	set_idt_entry(11, (uintptr_t)isr_11, 0x08, 0x8E);
	set_idt_entry(12, (uintptr_t)isr_12, 0x08, 0x8E);
	set_idt_entry(13, (uintptr_t)isr_13, 0x08, 0x8E);
	set_idt_entry(14, (uintptr_t)isr_14, 0x08, 0x8E);
	set_idt_entry(15, (uintptr_t)isr_15, 0x08, 0x8E);
	set_idt_entry(16, (uintptr_t)isr_16, 0x08, 0x8E);
	set_idt_entry(17, (uintptr_t)isr_17, 0x08, 0x8E);
	set_idt_entry(18, (uintptr_t)isr_18, 0x08, 0x8E);
	set_idt_entry(19, (uintptr_t)isr_19, 0x08, 0x8E);
	set_idt_entry(20, (uintptr_t)isr_20, 0x08, 0x8E);
	set_idt_entry(21, (uintptr_t)isr_21, 0x08, 0x8E);
	set_idt_entry(22, (uintptr_t)isr_22, 0x08, 0x8E);
	set_idt_entry(23, (uintptr_t)isr_23, 0x08, 0x8E);
	set_idt_entry(24, (uintptr_t)isr_24, 0x08, 0x8E);
	set_idt_entry(25, (uintptr_t)isr_25, 0x08, 0x8E);
	set_idt_entry(26, (uintptr_t)isr_26, 0x08, 0x8E);
	set_idt_entry(27, (uintptr_t)isr_27, 0x08, 0x8E);
	set_idt_entry(28, (uintptr_t)isr_28, 0x08, 0x8E);
	set_idt_entry(29, (uintptr_t)isr_29, 0x08, 0x8E);
	set_idt_entry(30, (uintptr_t)isr_30, 0x08, 0x8E);
	set_idt_entry(31, (uintptr_t)isr_31, 0x08, 0x8E);

	// Register the ISRs for interrupts 32 through 47
	// for hardware interrupts.
	set_idt_entry(32, (uintptr_t)irq_0, 0x08, 0x8E);
	set_idt_entry(33, (uintptr_t)irq_1, 0x08, 0x8E);
	set_idt_entry(34, (uintptr_t)irq_2, 0x08, 0x8E);
	set_idt_entry(35, (uintptr_t)irq_3, 0x08, 0x8E);
	set_idt_entry(36, (uintptr_t)irq_4, 0x08, 0x8E);
	set_idt_entry(37, (uintptr_t)irq_5, 0x08, 0x8E);
	set_idt_entry(38, (uintptr_t)irq_6, 0x08, 0x8E);
	set_idt_entry(39, (uintptr_t)irq_7, 0x08, 0x8E);
	set_idt_entry(40, (uintptr_t)irq_8, 0x08, 0x8E);
	set_idt_entry(41, (uintptr_t)irq_9, 0x08, 0x8E);
	set_idt_entry(42, (uintptr_t)irq_10, 0x08, 0x8E);
	set_idt_entry(43, (uintptr_t)irq_11, 0x08, 0x8E);
	set_idt_entry(44, (uintptr_t)irq_12, 0x08, 0x8E);
	set_idt_entry(45, (uintptr_t)irq_13, 0x08, 0x8E);
	set_idt_entry(46, (uintptr_t)irq_14, 0x08, 0x8E);
	set_idt_entry(47, (uintptr_t)irq_15, 0x08, 0x8E);

	idt_first.limit_size = sizeof(idt) - 1;
	idt_first.base_addr = (idt_entry*)(&idt);

	// Load the IDT.
	load_idt((idt_entry*)&idt_first);
}
