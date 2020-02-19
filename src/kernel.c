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
#include "kernel/gdt.h"
#include "kernel/task.h"

#include "libc/stdio.h"
#include "libc/stdlib.h"
#include "libc/string.h"


// The initial starting point of the kernel stack
volatile uint32_t init_esp;

void k_main(uint32_t magic, multiboot_info_t* mbi, uint32_t esp)
{
	uint8_t done;

	init_esp = esp;
	
	k_vga_init();
	k_pit_init();
	k_memory_init(mbi);
	k_init_tasking();

	printf("magic: %#X\n", magic);
	printf("stack location: %#X\n", esp);

	// Enter into the main loop.
	done = 0;
	while (!done)
	{
	
	}
}
