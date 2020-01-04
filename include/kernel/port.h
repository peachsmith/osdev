#ifndef JEP_K_PORT_H
#define JEP_K_PORT_H

#include <stdint.h>

/**
 * I/O port interface.
 * The function signatures in this file expose procedures that wrap the
 * IN and OUT x86 instructions.
 */

/**
 * Writes an 8-bit value to an I/O port.
 *
 * Params:
 *   uint16_t port - the I/O port to which a byte will be written
 *   uint8_t value - an 8-bit byte containing a value to be written
 */
extern void k_outb(uint16_t port, uint8_t value);

/**
 * Writes a 16-bit value to an I/O port.
 *
 * Params:
 *   uint16_t port - the I/O port to which a byte will be written
 *   uint16_t value - a 16-bit word containing a value to be written
 */
extern void k_outw(uint16_t port, uint8_t value);

/**
 * Reads an 8-bit value from an I/O port.
 *
 * Params:
 *   uint16_t port - the port number
 *
 * Returns:
 *   uint8_t the value read from the port
 */
extern uint8_t k_inb(uint16_t port);

/**
 * Reads a 16-bit value from an I/O port.
 *
 * Params:
 *   uint16_t port - the port number
 *
 * Returns:
 *   uint16_t the value read from the port
 */
extern uint16_t k_inw(uint16_t port);

#endif
