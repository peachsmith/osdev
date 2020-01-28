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
 * Enables paging.
 */
//extern void init_paging();

/**
 * Enables paging.
 */
extern void enable_paging();

/**
 * Disables paging.
 */
extern void disable_paging();

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
void* k_alloc();

/**
 * Writes a Multiboot memory map to a stream.
 * 
 * Params:
 *   FILE* - a pointer to a stream
 *   multiboot_info_t* - a pointer to a multiboot info struct
 */
void fprint_mmap(FILE* stream, multiboot_info_t* mbi);

#endif
