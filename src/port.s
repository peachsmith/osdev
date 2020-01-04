#=============================================================================
# I/O port implementation
# This file contains procedures that execute the IN and OUT instructions.
#=============================================================================

.section .text

.global k_outb
.global k_inb
.global k_outw
.global k_inw


##
# Writes an 8-bit byte to an I/O port.
#
# Params:
#   word - a 16-bit port number
#   byte - an 8-bit value to be written
#
k_outb:
	
	pushl %ebp
	movl %esp, %ebp
	subl $8, %esp
	
	movw 8(%ebp), %dx  # first argument (a word containing the port number)
	movb 12(%ebp), %al # second argument (a byte containing the value)

	out %al, %dx       # write the byte in al to the port in dx
	
	leave
	ret


##
# Writes a 16-bit word to an I/O port.
#
# Params:
#   word - a 16-bit port number
#   word - a 16-bit value to be written
#
k_outw:
	
	pushl %ebp
	movl %esp, %ebp
	subl $8, %esp
	
	movw 8(%ebp), %dx  # first argument (a word containing the port number)
	movb 12(%ebp), %al # second argument (a word containing the value)

	out %ax, %dx       # write the word in ax to the port in dx
	
	leave
	ret


##
# Reads an 8-bit byte from an I/O port.
#
# Params:
#   word - a 16-bit port number
#
# Returns:
#   byte - an 8-bit byte
#
k_inb:

	pushl %ebp
	movl %esp, %ebp
	subl $8, %esp
	
	movw 8(%ebp), %dx  # first argument (a word containing the port number)

	in %dx, %al        # read a byte from the I/O port into al
	
	leave
	ret


##
# Reads a 16-bit word from an I/O port.
#
# Params:
#   word - a 16-bit port number
#
# Returns:
#   word - a 16-bit word
#
k_inw:

	pushl %ebp
	movl %esp, %ebp
	subl $8, %esp
	
	movw 8(%ebp), %dx  # first argument (a word containing the port number)

	in %dx, %ax        # read a word from the I/O port into ax
	
	leave
	ret
