#ifndef JEP_K_MEMORY_H
#define JEP_K_MEMORY_H

#include <stdint.h>
#include <stddef.h>

#include "kernel/multiboot.h"
#include "libc/stdio.h"

/**
 * The memory management interface.
 */


/**
 * Initializes the memory service.
 */
void k_memory_init(multiboot_info_t* mbi);

/**
 * Attempts to allocate a single page of virtual memory.
 * 
 * Returns:
 *   void* - a pointer to virtual memory or NULL on failure
 */
void* k_palloc();

/**
 * Writes a Multiboot memory map to a stream.
 * 
 * Params:
 *   FILE* - a pointer to a stream
 *   multiboot_info_t* - a pointer to a multiboot info struct
 */
void fprint_mmap(FILE* stream, multiboot_info_t* mbi);

#endif
