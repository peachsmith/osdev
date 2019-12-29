.section text

.global my_outb

# This procedure executes the outb instruction.
my_outb:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
#	call	__x86.get_pc_thunk.ax
#	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	movb	%dl, -4(%ebp)
	movw	%ax, -8(%ebp)
	movzbl	-4(%ebp), %eax
	movzwl	-8(%ebp), %edx
	outb %al,%dx
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
	
	# According to the x86 System V ABI found at
	# http://www.sco.com/developers/devspecs/abi386-4.pdf
	# The first argument is located at offset 8(%ebp) and the
	# second argument is at offset 12(%ebp).
	# Since the C function signature is
	# extern void my_outb(uint16_t port, uint8_t value)
	# the port number should be at 8(%ebp) and the value should be at 12(%ebp).	
	
	#movw 8(%ebp), %dx  # first argument (a word containing the port number)
	#movb 12(%ebp), %al # second argument (a byte containing the value)
	#outb %al, %dx      # write the byte in al to the port in dx
