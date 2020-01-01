#include "serial.h"
#include "port.h"
#include "string.h"

#define COM1 0x3F8

void com1_init()
{
	k_outb(COM1 + 1, 0x00);    // Disable all interrupts
	k_outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	k_outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	k_outb(COM1 + 1, 0x00);    //                  (hi byte)
	k_outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	k_outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	k_outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

static uint8_t is_tx_empty(uint16_t port) {
	return k_inb(port + 5) & 0x20;
}

static void serial_write(uint16_t port, uint8_t a) {
	while (is_tx_empty(port) == 0);
	k_outb(port, a);
}

void com1_write(uint8_t d)
{
	serial_write(COM1, d);
}

void com1_writes(char* str)
{
	size_t len = strlen(str);

	for (size_t i = 0; i < len; i++)
		com1_write(COM1, str[i]);
}
