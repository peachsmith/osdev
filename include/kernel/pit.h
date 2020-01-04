#ifndef JEP_K_PIT_H
#define JEP_K_PIT_H

/**
 * The Programmable Interval Timer (PIT) interface.
 * Channel 0 on the PIT is connected to to IRQ0 and can be used for
 * implementing timers.
 */

#include <stdint.h>

/**
 * Initializes the PIT.
 */
void k_pit_init();

/**
 * Waits for an approximate number of seconds.
 * This function uses the PIT, so the actual amount of time waited is a rough
 * approximation.
 *
 * Params:
 *   uint32_t s - the number of seconds to wait
 */
void k_pit_waits(uint32_t s);

/**
 * Waits for an approximate number of milliseconds.
 * This function uses the PIT, so the actual amount of time waited is a rough
 * approximation.
 *
 * Params:
 *   uint32_t s - the number of milliseconds to wait
 */
void k_pit_waitm(uint32_t m);

#endif
