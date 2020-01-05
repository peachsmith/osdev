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
#include "kernel/memory.h"
#include "kernel/multiboot.h"


void kernel_main(uint32_t mb_magic, multiboot_info_t* mbi)
{
	vga_init();
	k_pit_init();
	
	vga_writes("Hello, World!\n");
	
	if (mb_magic == 0x2BADB002)
	{
		vga_writes("Obtained the multiboot magic number.\n");
	}
	else
	{
		vga_writes("Failed to get the multiboot magic number.\n");
	}

	
	// Attempt to allocate memory
	char* my_char = (char*)build_pointer(3);
	
	my_char[0] = 'A';
	my_char[1] = 'B';
	my_char[2] = 'C';
	
	vga_putchar(my_char[0]);
	vga_putchar('\n');
	
	vga_putchar(my_char[1]);
	vga_putchar('\n');
	
	vga_putchar(my_char[2]);
	vga_putchar('\n');

	uint8_t done = 0;
	while (!done)
	{
		// k_pit_waits test
		k_pit_waits(1);
		com1_writes("This is the main loop.\n");
	}
}
