#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// This tutorial will only work for the 32-bit ix86 targets.
#if !defined(__i386__)
#error "An ix86-elf compiler is required"
#endif




#include "kernel/vga.h"
#include "kernel/pit.h"
#include "kernel/memory.h"
#include "kernel/multiboot.h"

#include "libc/stdio.h"
#include "libc/stdlib.h"
#include "libc/string.h"

void k_main(multiboot_info_t* mbi)
{
	uint8_t done;
	
	k_vga_init();
	k_pit_init();
	k_memory_init(mbi);

	// Enter into the main loop.
	done = 0;
	while (!done)
	{
	
	}
}
