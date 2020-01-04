.section .text

.global load_gdt

load_gdt:
	pushl %ebp
	movl %esp, %ebp
	subl $0x08, %esp
	
	mov 8(%ebp), %eax
	lgdt (%eax)

	mov $0x10, %eax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	mov %eax, %ss
	
	jmp $0x8, $.long_jump

.long_jump:
	leave
	ret
