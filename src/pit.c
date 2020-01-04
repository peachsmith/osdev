#include "kernel/pit.h"
#include "kernel/port.h"

#define CHANNEL_0 0x40
#define COMMAND   0x43

void k_pit_init()
{
	// Due to historical reasons, a frequency of roughly 1.193186 MHz
	// is used by the PIT.
	uint32_t history; // historical frequency
	uint32_t divisor; // divisor used to get the desired frequency
	uint8_t pit_lo;   // low byte of divisor
	uint8_t pit_hi;   // high byte of divisor

	// Calculate the divisor.
	// Note that the divisor should be able to git within a word.
	history = 1193180;
	divisor = history / 1000;

	// Split the divisor into high and low bytes.
	pit_lo = (uint8_t)(divisor & 0xFF);
	pit_hi = (uint8_t)((divisor >> 8) & 0xFF);

	// Send the command byte.
	k_outb(COMMAND, 0x36);

	// Send the divisor to channel 0.
	k_outb(CHANNEL_0, pit_lo);
	k_outb(CHANNEL_0, pit_hi);
}
