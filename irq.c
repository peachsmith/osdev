#include <stdint.h>

#include "port.h"
#include "vga.h"

static uint32_t ticks = 0;
static uint32_t pit_i = 0;
char pit_chars[10] = {
	'a', 'b', 'c', 'd', 'e',
	'f', 'g', 'h', 'i', 'j'
};

void irq0_handler() {

	// Temporary PIT handling to test interrupts
	if (ticks < 200)
	{
		ticks++;
		if (ticks % 20 == 0 && pit_i < 10)
		{
			vga_putchar(pit_chars[pit_i++]);
			vga_putchar('\n');
		}
	}
	
	k_outb(0x20, 0x20);
}

void irq1_handler() {
	k_outb(0x20, 0x20);
}

void irq2_handler() {
	k_outb(0x20, 0x20);
}

void irq3_handler() {
	k_outb(0x20, 0x20);
}

void irq4_handler() {
	k_outb(0x20, 0x20);
}

void irq5_handler() {
	k_outb(0x20, 0x20);
}

void irq6_handler() {
	k_outb(0x20, 0x20);
}

void irq7_handler() {
	k_outb(0x20, 0x20);
}

void irq8_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq9_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq10_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq11_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq12_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq13_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq14_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq15_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}
