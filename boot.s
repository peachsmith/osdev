#==============================================================================
# Bootstrap code based on the boot.s file from the OSDev wiki.
#==============================================================================

# Multiboot header constants
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot
 
# Declare a multiboot header.
# It will be within the first 8 KiB of the kernel file aligned at a 32-bit
# boundary. It's given its own section so it can be forced into the first
# 8 KiB of the kernel file.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
 
# Create the stack.
# Create a symbol to represent the bottom of the stack, skip 16384 bytes,
# then create a symbol to represent the top of the stack.
# The x86 stack must be 16-byte aligned, according to the System V ABI.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
 
# The linker script specifies _start as the entry point into the kernel.
.section .text
.global _start
.extern init_gdt
.type _start, @function
_start:

	# The bootloader has loaded into 32-bit protected mode on x86.
	# Interrupts are currently disabled.
	# Paging is currently disabled.
	# Floating point instructions have not yet been initialized.
 
	# Set up the stack by moving the top of the stack into the stack pointer.
	mov $stack_top, %esp
 
	# Loan the GDT
	call init_gdt
	
	# TODO: enable paging
 
	# Enter the kernel
	call kernel_main
 
	# If we've exited from the kernel,
	# put the computer into an infinite loop.
	cli    # Disable interrupts
1:	hlt    # Wait for next interrupt
	jmp 1b # Jump to the hlt instruction if an interrupt is encountered

# Set the size of the _start symbol to the current location to be
# the current location minus the start.
.size _start, . - _start
