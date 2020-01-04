.section .text

.global load_page_directory
.global enable_paging

load_page_directory:
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax
	movl %eax, %cr3

	leave
	ret

enable_paging:
	pushl %ebp
	movl %esp, %ebp

	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0

	leave
	ret
