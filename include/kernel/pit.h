#ifndef PIT_H
#define PIT_H

#include <stdint.h>

/**
 * Waits for an approximate number of seconds.
 * This function uses the PIT, so the actual amount
 * of time waited is a rough approximation.
 *
 * Params:
 *   uint16_t s - the number of seconds to wait
 */
void k_wait(uint16_t s);

#endif
