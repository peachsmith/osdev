#ifndef JEP_K_STDLIB_H
#define JEP_K_STDLIB_H

#include <stddef.h>

/**
 * Allocates a block of memory.
 * 
 * Params:
 *   size_t the number of bytes to allocate
 * 
 * Returns:
 *   void* - a pointer to a block of memory or NULL on failure
 */
void* malloc(size_t n);

#endif
