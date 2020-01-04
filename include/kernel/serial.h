#ifndef JEP_K_SERIAL_H
#define JEP_K_SERIAL_H

#include <stdint.h>

/**
 * The serial I/O interface.
 * Communication over legacy serial ports, in this context, is used primarily
 * for development and debugging.
 */

/**
 * Initializes COM1.
 */
void com1_init();

/**
 * Writes a single 8-bit byte to COM1.
 *
 * Params:
 *   uint8_t - an 8-bit byte
 */
void com1_write(uint8_t d);

/**
 * Writes a NUL-terminated character array to COM1,
 * not including the terminating NUL character.
 *
 * Params:
 *   char* - a pointer to a character array
 */
void com1_writes(char* str);

#endif
