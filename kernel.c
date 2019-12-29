#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// This tutorial will only work for the 32-bit ix86 targets.
#if !defined(__i386__)
#error "An ix86-elf compiler is required"
#endif


#include "port.h"
#include "vga.h"


void kernel_main(void)
{
	vga_set_cursor(0, 0);

	// Initialize VGA terminal
	vga_init();

	// Example VGA output
	vga_writes("Hello, World!\n");
	vga_writes("We're still in VGA text mode with no interesting stuff yet.\n");
}
