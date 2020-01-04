#include <stdint.h>

#include "kernel/port.h"
#include "kernel/vga.h"
#include "kernel/pit.h"
#include "kernel/serial.h"

static volatile uint32_t ticks = 0;
static volatile uint32_t pit_s = 0;

void k_wait(uint16_t s)
{
	uint32_t end = pit_s + s;

	while (pit_s < end);
}

void isr_0_handler()
{
	com1_writes("FAULT: Divide Error\n");
}

void isr_1_handler()
{
	com1_writes("FAULT/TRAP: Debug Exception\n");
}

void isr_2_handler()
{
	com1_writes("INTERRUPT: NMI Interrupt\n");
}

void isr_3_handler()
{
	// TRAP: Breakpoint
}

void isr_4_handler()
{
	com1_writes("TRAP: Overflow\n");
}

void isr_5_handler()
{
	com1_writes("FAULT: BOUND Range Exceeded\n");
}

void isr_6_handler()
{
	com1_writes("FAULT: Invalid Opcode\n");
}

void isr_7_handler()
{
	com1_writes("FAULT: Device Not Available\n");
}

void isr_8_handler()
{
	com1_writes("ABORT: Double Fault\n");
}

void isr_9_handler()
{
	// Reserved
	com1_writes("FAULT: Coprocessor Segment Overrun\n");
}

void isr_10_handler()
{
	com1_writes("FAULT: Invalid TSS\n");
}
void isr_11_handler()
{
	com1_writes("FAULT: Segment Not Present\n");
}

void isr_12_handler()
{
	com1_writes("FAULT: Stack Segment Fault\n");
}

void isr_13_handler()
{
	com1_writes("FAULT: General Protection\n");
}

void isr_14_handler()
{
	com1_writes("FAULT: Page Fault\n");
}

void isr_15_handler()
{
	// Reserved
}

void isr_16_handler()
{
	com1_writes("FAULT: x87 FPU Floating Point Error\n");
}

void isr_17_handler()
{
	com1_writes("FAULT: Alignment Check\n");
}

void isr_18_handler()
{
	com1_writes("ABORT: Machine Check\n");
}

void isr_19_handler()
{
	com1_writes("FAULT: SIMD Floating Point Exception\n");
}

void isr_20_handler()
{
	com1_writes("FAULT: Virtualization Exception\n");
}

void isr_21_handler()
{
	// Reserved
}

void isr_22_handler()
{
	// Reserved
}

void isr_23_handler()
{
	// Reserved
}

void isr_24_handler()
{
	// Reserved
}

void isr_25_handler()
{
	// Reserved
}

void isr_26_handler()
{
	// Reserved
}

void isr_27_handler()
{
	// Reserved
}

void isr_28_handler()
{
	// Reserved
}

void isr_29_handler()
{
	// Reserved
}

void isr_30_handler()
{
	// Reserved
}

void isr_31_handler()
{
	// Reserved
}



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
