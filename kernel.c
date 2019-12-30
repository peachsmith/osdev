#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// This tutorial will only work for the 32-bit ix86 targets.
#if !defined(__i386__)
#error "An ix86-elf compiler is required"
#endif


#include "port.h"
#include "vga.h"
#include "pit.h"

/**
 * Temporary COM1 initialization code
 */
void serial_init()
{
	uint16_t port =  0x3f8;
	k_outb(port + 1, 0x00);    // Disable all interrupts
	k_outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	k_outb(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	k_outb(port + 1, 0x00);    //                  (hi byte)
	k_outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
	k_outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	k_outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

uint8_t is_tx_empty(uint16_t port) {
	return k_inb(port + 5) & 0x20;
}
 
void serial_write(uint16_t port, char a) {
	while (is_tx_empty(port) == 0);
	k_outb(port, a);
}

/**
 * Some temporary PIT initialization code to confirm the functionality
 * of interrupt handling.
 */
void pit_init(uint16_t f)
{
	// Due to historical reasons, a frequency of roughly 1.193186 MHz
	// is used by the PIT.
	uint32_t history; // historical frequency
	uint32_t divisor; // divisor used to get the desired frequency
	uint8_t pit_lo;   // low byte of divisor
	uint8_t pit_hi;   // high byte of divisor
	
	// Calculate the divisor.
	// Note that the divisor should be able to git within a word.
	history = 1193180;
	divisor = history / f;
	
	// Split the divisor into high and low bytes.
	pit_lo = (uint8_t)(divisor & 0xFF);
	pit_hi = (uint8_t)((divisor >> 8) & 0xFF);

	// Send the command byte.
	k_outb(0x43, 0x36);

	// Send the divisor to channel 0.
	k_outb(0x40, pit_lo);
	k_outb(0x40, pit_hi);
}

void kernel_main(void)
{
	// Initialize VGA terminal
	vga_init();

	// Set the cursor position to row 0, column 2
	vga_set_cursor(13, 0);

	// Example VGA output
	vga_writes("Hello, World!\n");
	vga_writes("We're still in VGA text mode with no interesting stuff yet.\n");
	
	pit_init(20);
	
	k_wait(2);
	vga_writes("Hello, again!\n");
	k_wait(10);
	vga_writes("Hello, again!\n");
	
	serial_write(0x3f8, 'H');
	serial_write(0x3f8, 'e');
	serial_write(0x3f8, 'l');
	serial_write(0x3f8, 'l');
	serial_write(0x3f8, 'o');
	serial_write(0x3f8, ',');
	serial_write(0x3f8, ' ');
	serial_write(0x3f8, 's');
	serial_write(0x3f8, 'e');
	serial_write(0x3f8, 'r');
	serial_write(0x3f8, 'i');
	serial_write(0x3f8, 'a');
	serial_write(0x3f8, 'l');
	serial_write(0x3f8, '!');
	
	uint8_t done = 0;
	while(!done)
	{
		// Do things
	}
}
