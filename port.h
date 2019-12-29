#ifndef PORT_H
#define PORT_H

/**
 * I/O port interface.
 * These functions wrap the IN and OUT x86 instructions.
 */

/**
 * This signature exposes an assembly procedure that executes the outb
 * instruction to write an 8-bit value to a 16-bit port.
 *
 * Params:
 *   uint16_t port - the I/O port to which a byte will be written
 *   uint8_t value - an 8-bit byte containing a value to be written
 */
extern void k_outb(uint16_t port, uint8_t value);

#endif
