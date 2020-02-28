#include <stdint.h>

#include "kernel/port.h"
#include "kernel/pit.h"
#include "kernel/task.h"
#include "libc/stdio.h"
#include "libc/stdlib.h"

/**
 * A macro to put the computer into an infinite loop.
 * This is currently only used for development and debugging.
 * Future implementations may be more elaborate and have
 */
#define HANG for(;;)

volatile uint32_t main_ticks = 0;

void k_pit_waits(uint32_t s)
{
	uint32_t ticks = s * 1000;
	ticks = main_ticks + ticks;
	
	while(main_ticks < ticks);
}

void k_pit_waitm(uint32_t m)
{
	uint32_t ticks = m;
	ticks = main_ticks + ticks;
	
	while(main_ticks < ticks);
}

void isr_0_handler()
{
	fprintf(stddbg, "FAULT: Divide Error\n");
	HANG;
}

void isr_1_handler()
{
	fprintf(stddbg, "FAULT/TRAP: Debug Exception\n");
}

void isr_2_handler()
{
	fprintf(stddbg, "INTERRUPT: NMI Interrupt\n");
}

void isr_3_handler()
{
	// TRAP: Breakpoint
}

void isr_4_handler()
{
	fprintf(stddbg, "TRAP: Overflow\n");
}

void isr_5_handler()
{
	fprintf(stddbg, "FAULT: BOUND Range Exceeded\n");
	HANG;
}

void isr_6_handler()
{
	fprintf(stddbg, "FAULT: Invalid Opcode\n");
	HANG;
}

void isr_7_handler()
{
	fprintf(stddbg, "FAULT: Device Not Available\n");
}

void isr_8_handler()
{
	fprintf(stddbg, "ABORT: Double Fault\n");
}

void isr_9_handler()
{
	// Reserved
	fprintf(stddbg, "FAULT: Coprocessor Segment Overrun\n");
}

void isr_10_handler()
{
	fprintf(stddbg, "FAULT: Invalid TSS\n");
	HANG;
}
void isr_11_handler()
{
	fprintf(stddbg, "FAULT: Segment Not Present\n");
	HANG;
}

void isr_12_handler()
{
	fprintf(stddbg, "FAULT: Stack Segment Fault\n");
	HANG;
}

void isr_13_handler(uint32_t err, uint32_t eip)
{
	
	fprintf(stddbg, "FAULT: General Protection err: %X, eip: %X\n", err, eip);
	if (err != 0)
	{
		//uint32_t index = 
		//uint32_t table = ;
		fprintf(stddbg, "Table: %d, index: %d\n",
			(err & 6) >> 1,
			(err & 0xFFFF) >> 3);
	}

	HANG;
}

void isr_14_handler()
{
	fprintf(stddbg, "FAULT: Page Fault\n");
	HANG;
}

void isr_15_handler()
{
	// Reserved
}

void isr_16_handler()
{
	fprintf(stddbg, "FAULT: x87 FPU Floating Point Error\n");
}

void isr_17_handler()
{
	fprintf(stddbg, "FAULT: Alignment Check\n");
}

void isr_18_handler()
{
	fprintf(stddbg, "ABORT: Machine Check\n");
}

void isr_19_handler()
{
	fprintf(stddbg, "FAULT: SIMD Floating Point Exception\n");
	HANG;
}

void isr_20_handler()
{
	fprintf(stddbg, "FAULT: Virtualization Exception\n");
	HANG;
}

void isr_21_handler()
{
	// Reserved
}

void isr_22_handler()
{
	// Reserved
}

void isr_23_handler()
{
	// Reserved
}

void isr_24_handler()
{
	// Reserved
}

void isr_25_handler()
{
	// Reserved
}

void isr_26_handler()
{
	// Reserved
}

void isr_27_handler()
{
	// Reserved
}

void isr_28_handler()
{
	// Reserved
}

void isr_29_handler()
{
	// Reserved
}

void isr_30_handler()
{
	// Reserved
}

void isr_31_handler()
{
	// Reserved
}




uint32_t irq_0_handler(uint32_t real_esp)
{
	// if (ticks > 0)
	// 	ticks--;

	main_ticks++;

	// Send the EOI before doing task switch logic
	// since we don't normally return from a task switch.
	k_outb(0x20, 0x20);

	// Here is where we initiate a task switch
	uint32_t next_esp = k_switch_task(main_ticks, real_esp);

	return next_esp;
}

void irq_1_handler()
{
	//uint8_t b = k_inb(0x60);

	//fprintf(stddbg, "key: %X\n", b);

	k_outb(0x20, 0x20);
}

void irq_2_handler()
{
	k_outb(0x20, 0x20);
}

void irq_3_handler()
{
	k_outb(0x20, 0x20);
}

void irq_4_handler()
{
	k_outb(0x20, 0x20);
}

void irq_5_handler()
{
	k_outb(0x20, 0x20);
}

void irq_6_handler()
{
	k_outb(0x20, 0x20);
}

void irq_7_handler()
{
	k_outb(0x20, 0x20);
}

void irq_8_handler()
{
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_9_handler()
{
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_10_handler()
{
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_11_handler()
{
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_12_handler()
{
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_13_handler()
{
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_14_handler()
{
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_15_handler()
{
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}
