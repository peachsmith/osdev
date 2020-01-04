#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

void com1_init();

void com1_write(uint8_t d);

void com1_writes(char* str);

#endif
