.section .text

.global k_outb
.global k_inb
.global k_int_test

# Writes a byte to an output port.
#
# Params:
#   word - the port number to which the byte will be written
#   byte - an 8-bit number to be written to the port
k_outb:
	
	pushl %ebp
	movl %esp, %ebp
	subl $8, %esp
	
	movw 8(%ebp), %dx  # first argument (a word containing the port number)
	movb 12(%ebp), %al # second argument (a byte containing the value)

	out %al, %dx       # write the byte in al to the port in dx
	
	leave
	ret
	
k_inb:

	pushl %ebp
	movl %esp, %ebp
	subl $8, %esp
	
	movw 8(%ebp), %dx  # first argument (a word containing the port number)

	in %dx, %al       # write the byte in al to the port in dx
	
	leave
	ret

k_int_test:

	pushl %ebp
	movl %esp, %ebp
	subl $8, %esp
	
	movb $0, %al
	movb $0, %cl
	div %cl
	
	leave
	ret
