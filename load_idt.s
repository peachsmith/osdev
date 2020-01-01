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
	pusha
	call isr_0_handler
	popa
	iret

isr_1:
	pusha
	call isr_1_handler
	popa
	iret

isr_2:
	pusha
	call isr_2_handler
	popa
	iret

isr_3:
	pusha
	call isr_3_handler
	popa
	iret

isr_4:
	pusha
	call isr_4_handler
	popa
	iret

isr_5:
	pusha
	call isr_5_handler
	popa
	iret

isr_6:
	pusha
	call isr_6_handler
	popa
	iret

isr_7:
	pusha
	call isr_7_handler
	popa
	iret

isr_8:
	pusha
	call isr_8_handler
	popa
	iret

isr_9:
	pusha
	call isr_9_handler
	popa
	iret

isr_10:
	pusha
	call isr_10_handler
	popa
	iret

isr_11:
	pusha
	call isr_11_handler
	popa
	iret

isr_12:
	pusha
	call isr_12_handler
	popa
	iret

isr_13:
	pusha
	call isr_13_handler
	popa
	iret

isr_14:
	pusha
	call isr_14_handler
	popa
	iret

isr_15:
	pusha
	call isr_15_handler
	popa
	iret

isr_16:
	pusha
	call isr_16_handler
	popa
	iret

isr_17:
	pusha
	call isr_17_handler
	popa
	iret

isr_18:
	pusha
	call isr_18_handler
	popa
	iret

isr_19:
	pusha
	call isr_19_handler
	popa
	iret

isr_20:
	pusha
	call isr_20_handler
	popa
	iret

isr_21:
	pusha
	call isr_21_handler
	popa
	iret

isr_22:
	pusha
	call isr_22_handler
	popa
	iret

isr_23:
	pusha
	call isr_23_handler
	popa
	iret

isr_24:
	pusha
	call isr_24_handler
	popa
	iret

isr_25:
	pusha
	call isr_25_handler
	popa
	iret

isr_26:
	pusha
	call isr_26_handler
	popa
	iret

isr_27:
	pusha
	call isr_27_handler
	popa
	iret

isr_28:
	pusha
	call isr_28_handler
	popa
	iret

isr_29:
	pusha
	call isr_29_handler
	popa
	iret

isr_30:
	pusha
	call isr_30_handler
	popa
	iret

isr_31:
	pusha
	call isr_31_handler
	popa
	iret



#-----------------------------------------------------------------------------
# IRQ handlers
#-----------------------------------------------------------------------------

irq_0:
	pusha
	call irq_0_handler
	popa
	iret
 
irq_1:
	pusha
	call irq_1_handler
	popa
	iret
 
irq_2:
	pusha
	call irq_2_handler
	popa
	iret
 
irq_3:
	pusha
	call irq_3_handler
	popa
	iret
 
irq_4:
	pusha
	call irq_4_handler
	popa
	iret
 
irq_5:
	pusha
	call irq_5_handler
	popa
	iret
 
irq_6:
	pusha
	call irq_6_handler
	popa
	iret
 
irq_7:
	pusha
	call irq_7_handler
	popa
	iret
 
irq_8:
	pusha
	call irq_8_handler
	popa
	iret
 
irq_9:
	pusha
	call irq_9_handler
	popa
	iret
 
irq_10:
	pusha
	call irq_10_handler
	popa
	iret
 
irq_11:
	pusha
	call irq_11_handler
	popa
	iret
 
irq_12:
	pusha
	call irq_12_handler
	popa
	iret
 
irq_13:
	pusha
	call irq_13_handler
	popa
	iret
 
irq_14:
	pusha
	call irq_14_handler
	popa
	iret
 
irq_15:
	pusha
	call irq_15_handler
	popa
	iret

load_idt:
	mov 4(%esp), %eax
	lidt (%eax)
	sti
	ret
