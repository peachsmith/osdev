#ifndef STRING_H
#define STRING_H

/**
 * Basic C string functions
 */

#include <stddef.h>

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
