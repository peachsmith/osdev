.section .text

.global load_gdt

load_gdt:
	mov 4(%esp), %eax
	lgdt (%eax)

	mov $0x10, %eax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	mov %eax, %ss
	jmp $0x8, $.long_jump # this might not be necessary. we may be able to do a regular ret

.long_jump:
	ret
