#ifndef JEP_K_STRING_H
#define JEP_K_STRING_H

/**
 * Basic C string functions
 */

#include <stddef.h>

/**
 * Copies the values of n bytes from a source to a destination.
 * The memory blocks pointed to by the source and destination pointers
 * should not overlap.
 * 
 * Params:
 *   void* - the destination memory
 *   void* - the source memory
 *   size_t - the number of bytes to copy
 * 
 * Returns:
 *   void* - the destination pointer
 */
void* memcpy(void* dest, const void* src, size_t n);

/**
 * Copies the values of n bytes from a source to a destination.
 * The memory blocks pointed to by the source and destination pointers
 * may overlap.
 * 
 * Params:
 *   void* - the destination memory
 *   void* - the source memory
 *   size_t - the number of bytes to copy
 * 
 * Returns:
 *   void* - the destination pointer
 */
void* memmove(void* dest, const void* src, size_t n);

/**
 * Determines the length of a NUL-terminated sequence of characters.
 * The length does not include the NUL character.
 *
 * Params:
 *   const char* - a pointer to an array of characters
 *
 * Returns:
 *   size_t - the length of the character array not including the NUL
 *            character.
 */
size_t strlen(const char* str);

#endif
