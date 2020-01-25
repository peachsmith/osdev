#=============================================================================
# Paging initialization
# This file contains procedures for enabling paging.
#=============================================================================

.section .text

.global load_page_directory
.global enable_paging
.global disable_paging


##
# Loads the address of a page directory into the CR3 register.
#
# Params:
#   word - the memory address of a page directory
#
load_page_directory:
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax
	movl %eax, %cr3

	leave
	ret


##
# Enables paging
#
enable_paging:
	pushl %ebp
	movl %esp, %ebp

	# Set the paging enable bit in CR0
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0

	leave
	ret

##
# Disables paging
#
disable_paging:
	pushl %ebp
	movl %esp, %ebp

	# Clear the paging enable bit in CR0
	mov %cr0, %eax
	xor $0x80000000, %eax
	mov %eax, %cr0

	leave
	ret
