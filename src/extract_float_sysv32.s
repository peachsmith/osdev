.section .text

.global extract_float_sysv32
.global extract_double_sysv32

##
# Extracts the raw binary data of an IEEE 754 floating point number
# into a 32-bit unsigned integer.
# This procedure is intended for use on 32-bit x86 platform using the
# System V ABI.
#
# Params:
#   float - a floating point number
#
# Returns:
#   dword - a 32-bit unsigned integer
extract_float_sysv32:
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax	

	leave
	ret

##
# Extracts the raw binary data of an IEEE 754 floating point number
# into a 64-bit unsigned integer.
# This procedure is intended for use on 32-bit x86 platform using the
# System V ABI.
#
# Params:
#   double - a double precision floating point number
#
# Returns:
#   eax - the low 32 bits of a double precision floating point number
#   edx - the high 32 bits of a double precision floating point number
extract_double_sysv32:
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax  # low bits
	movl 12(%ebp), %edx # hi bits

	leave
	ret
