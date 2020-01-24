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

#include "libc/stdio.h"


void kernel_main(uint32_t mb_magic, multiboot_info_t* mbi)
{
	vga_init();
	k_pit_init();
	
	printf("Multiboot magic number: %X\n", mb_magic);
	printf("mmap length: %d\n", mbi->mmap_length);
	printf("mmap address: %X\n", mbi->mmap_addr);
	
	// 1MiB = 0x100000

	multiboot_memory_map_t* mmm;

	for (mmm = (multiboot_memory_map_t*)mbi->mmap_addr;
		(uint32_t)mmm < (mbi->mmap_addr + mbi->mmap_length);
		mmm = (multiboot_memory_map_t*)((uint32_t)mmm + mmm->size + sizeof(mmm->size)))
	{
		printf("addr: %016llX, len: %016llX, type: %4d\n",
			(uint64_t)((mmm->addr_hi << 32) | mmm->addr_lo),
			(uint64_t)((mmm->len_hi << 32) | mmm->len_lo),
			mmm->type);
	}
	

	uint8_t done = 0;
	while (!done)
	{
		// k_pit_waits test
		//k_pit_waits(1);
		//com1_writes("This is the main loop.\n");
	}
}
