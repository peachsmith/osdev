.section .text

.global isr_0
.global isr_1
.global isr_2
.global isr_3
.global isr_4
.global isr_5
.global isr_6
.global isr_7
.global isr_8
.global isr_9
.global isr_10
.global isr_11
.global isr_12
.global isr_13
.global isr_14
.global isr_15
.global isr_16
.global isr_17
.global isr_18
.global isr_19
.global isr_20
.global isr_21
.global isr_22
.global isr_23
.global isr_24
.global isr_25
.global isr_26
.global isr_27
.global isr_28
.global isr_29
.global isr_30
.global isr_31

.global irq_0
.global irq_1
.global irq_2
.global irq_3
.global irq_4
.global irq_5
.global irq_6
.global irq_7
.global irq_8
.global irq_9
.global irq_10
.global irq_11
.global irq_12
.global irq_13
.global irq_14
.global irq_15

.global load_idt


#-----------------------------------------------------------------------------
# Handlers for exceptions and reserved interrupts
#-----------------------------------------------------------------------------
.extern isr_0_handler
.extern isr_1_handler
.extern isr_2_handler
.extern isr_3_handler
.extern isr_4_handler
.extern isr_5_handler
.extern isr_6_handler
.extern isr_7_handler
.extern isr_8_handler
.extern isr_9_handler
.extern isr_10_handler
.extern isr_11_handler
.extern isr_12_handler
.extern isr_13_handler
.extern isr_14_handler
.extern isr_15_handler
.extern isr_16_handler
.extern isr_17_handler
.extern isr_18_handler
.extern isr_19_handler
.extern isr_20_handler
.extern isr_21_handler
.extern isr_22_handler
.extern isr_23_handler
.extern isr_24_handler
.extern isr_25_handler
.extern isr_26_handler
.extern isr_27_handler
.extern isr_28_handler
.extern isr_29_handler
.extern isr_30_handler
.extern isr_31_handler


#-----------------------------------------------------------------------------
# IRQ handler functions
#-----------------------------------------------------------------------------
.extern irq_0_handler
.extern irq_1_handler
.extern irq_2_handler
.extern irq_3_handler
.extern irq_4_handler
.extern irq_5_handler
.extern irq_6_handler
.extern irq_7_handler
.extern irq_8_handler
.extern irq_9_handler
.extern irq_10_handler
.extern irq_11_handler
.extern irq_12_handler
.extern irq_13_handler
.extern irq_14_handler
.extern irq_15_handler


#-----------------------------------------------------------------------------
# ISRs for exceptions and reserved interrupts
#-----------------------------------------------------------------------------
isr_0:
	pushal
	call isr_0_handler
	popal
	iret

isr_1:
	pushal
	call isr_1_handler
	popal
	iret

isr_2:
	pushal
	call isr_2_handler
	popal
	iret

isr_3:
	pushal
	call isr_3_handler
	popal
	iret

isr_4:
	pushal
	call isr_4_handler
	popal
	iret

isr_5:
	pushal
	call isr_5_handler
	popal
	iret

isr_6:
	pushal
	call isr_6_handler
	popal
	iret

isr_7:
	pushal
	call isr_7_handler
	popal
	iret

isr_8:
	pushal
	call isr_8_handler
	popal
	iret

isr_9:
	pushal
	call isr_9_handler
	popal
	iret

isr_10:
	pushal
	call isr_10_handler
	popal
	iret

isr_11:
	pushal
	call isr_11_handler
	popal
	iret

isr_12:
	pushal
	call isr_12_handler
	popal
	iret

isr_13:
	pushal
	call isr_13_handler
	popal
	iret

isr_14:
	pushal
	call isr_14_handler
	popal
	iret

isr_15:
	pushal
	call isr_15_handler
	popal
	iret

isr_16:
	pushal
	call isr_16_handler
	popal
	iret

isr_17:
	pushal
	call isr_17_handler
	popal
	iret

isr_18:
	pushal
	call isr_18_handler
	popal
	iret

isr_19:
	pushal
	call isr_19_handler
	popal
	iret

isr_20:
	pushal
	call isr_20_handler
	popal
	iret

isr_21:
	pushal
	call isr_21_handler
	popal
	iret

isr_22:
	pushal
	call isr_22_handler
	popal
	iret

isr_23:
	pushal
	call isr_23_handler
	popal
	iret

isr_24:
	pushal
	call isr_24_handler
	popal
	iret

isr_25:
	pushal
	call isr_25_handler
	popal
	iret

isr_26:
	pushal
	call isr_26_handler
	popal
	iret

isr_27:
	pushal
	call isr_27_handler
	popal
	iret

isr_28:
	pushal
	call isr_28_handler
	popal
	iret

isr_29:
	pushal
	call isr_29_handler
	popal
	iret

isr_30:
	pushal
	call isr_30_handler
	popal
	iret

isr_31:
	pushal
	call isr_31_handler
	popal
	iret



#-----------------------------------------------------------------------------
# IRQ handlers
#-----------------------------------------------------------------------------

irq_0:
	pushal
	call irq_0_handler
	popal
	iret
 
irq_1:
	pushal
	call irq_1_handler
	popal
	iret
 
irq_2:
	pushal
	call irq_2_handler
	popal
	iret
 
irq_3:
	pushal
	call irq_3_handler
	popal
	iret
 
irq_4:
	pushal
	call irq_4_handler
	popal
	iret
 
irq_5:
	pushal
	call irq_5_handler
	popal
	iret
 
irq_6:
	pushal
	call irq_6_handler
	popal
	iret
 
irq_7:
	pushal
	call irq_7_handler
	popal
	iret
 
irq_8:
	pushal
	call irq_8_handler
	popal
	iret
 
irq_9:
	pushal
	call irq_9_handler
	popal
	iret
 
irq_10:
	pushal
	call irq_10_handler
	popal
	iret
 
irq_11:
	pushal
	call irq_11_handler
	popal
	iret
 
irq_12:
	pushal
	call irq_12_handler
	popal
	iret
 
irq_13:
	pushal
	call irq_13_handler
	popal
	iret
 
irq_14:
	pushal
	call irq_14_handler
	popal
	iret
 
irq_15:
	pushal
	call irq_15_handler
	popal
	iret

load_idt:
	pushl %ebp
	movl %esp, %ebp
	subl $0x08, %esp
	
	mov 8(%ebp), %eax
	lidt (%eax)
	sti
	
	leave
	ret
