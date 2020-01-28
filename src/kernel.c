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

void print_addr(void* a)
{
	uint32_t v = (uint32_t)a;

	printf("%5d %5d %5d\n", (v & (0xFFC00000)), (v & (0x3FF000)), (v & 0xFFF));
}

void k_main(multiboot_info_t* mbi)
{
	uint8_t done;
	
	k_vga_init();
	k_pit_init();
	k_memory_init(mbi);

	fprint_mmap(stdout, mbi);

	// Attempt to allocate 4 bytes.
	char* str = (char*)malloc(4);

	if (str == NULL)
	{
		printf("failed to allocate 4 bytes\n");
	}
	else
	{
		print_addr((void*)str);

		str[0] = 'A';
		str[1] = 'B';
		str[2] = 'C';
		str[3] = '\0';

		printf("dynamic string: %s\n", str);
	}
	

	// Enter into the main loop.
	done = 0;
	while (!done)
	{
	
	}
}
