#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// This tutorial will only work for the 32-bit ix86 targets.
#if !defined(__i386__)
#error "An ix86-elf compiler is required"
#endif


#include "kernel/port.h"
#include "kernel/vga.h"
#include "kernel/pit.h"
#include "kernel/serial.h"


void kernel_main(void)
{
	// Initialize VGA terminal
	vga_init();

	// Set the cursor position to row 0, column 2
	vga_set_cursor(13, 0);

	// Example VGA output
	vga_writes("Hello, World!\n");

	k_pit_init();

	uint8_t done = 0;
	while (!done)
	{
		// Do things
		k_pit_waits(2);
		com1_writes("This is the main loop.\n");
	}
}
