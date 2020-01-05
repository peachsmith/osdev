#ifndef JEP_K_MEMORY_H
#define JEP_K_MEMORY_H

#include <stdint.h>
#include <stddef.h>

/**
 * The memory management interface.
 */

/**
 * Enables paging.
 */
extern void init_paging();

/**
 * My attempt at malloc.
 */
void* build_pointer(size_t n);

#endif
