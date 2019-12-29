.section text

.global my_outb

# This procedure executes the outb instruction.
my_outb:
	
	# According to the x86 System V ABI found at
	# http://www.sco.com/developers/devspecs/abi386-4.pdf
	# The first argument is located at offset 8(%ebp) and the
	# second argument is at offset 12(%ebp).
	# Since the C function signature is
	# extern void my_outb(uint16_t port, uint8_t value)
	# the port number should be at 8(%ebp) and the value should be at 12(%ebp).	
	
	movw 8(%ebp), %dx  # first argument (a word containing the port number)
	movb 12(%ebp), %al # second argument (a byte containing the value)
	outb %al, %dx      # write the byte in al to the port in dx
