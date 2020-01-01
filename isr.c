#include <stdint.h>

#include "port.h"
#include "vga.h"
#include "pit.h"
#include "serial.h"

static volatile uint32_t ticks = 0;
static volatile uint32_t pit_s = 0;

void k_wait(uint16_t s)
{
	uint32_t end = pit_s + s;

	while (pit_s < end);
}

void isr_0_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {
	com1_writes("a divide by zero exception has occurred.\n");
}
void isr_1_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_2_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_3_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_4_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_5_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_6_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_7_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_8_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_9_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_10_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_11_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_12_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_13_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_14_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_15_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_16_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_17_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_18_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_19_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_20_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_21_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_22_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_23_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_24_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_25_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_26_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_27_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_28_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_29_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_30_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}
void isr_31_handler(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t ebx, uint32_t esp, uint32_t esi, uint32_t edi, uint32_t err) {}



void irq_0_handler() {

	ticks++;
	if (ticks % 20 == 0)
	{
		pit_s++;
	}

	k_outb(0x20, 0x20);
}

void irq_1_handler() {
	k_outb(0x20, 0x20);
}

void irq_2_handler() {
	k_outb(0x20, 0x20);
}

void irq_3_handler() {
	k_outb(0x20, 0x20);
}

void irq_4_handler() {
	k_outb(0x20, 0x20);
}

void irq_5_handler() {
	k_outb(0x20, 0x20);
}

void irq_6_handler() {
	k_outb(0x20, 0x20);
}

void irq_7_handler() {
	k_outb(0x20, 0x20);
}

void irq_8_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_9_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_10_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_11_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_12_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_13_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_14_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}

void irq_15_handler() {
	k_outb(0xA0, 0x20);
	k_outb(0x20, 0x20);
}
