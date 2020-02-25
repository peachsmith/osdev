.section .text

.global k_cli
.global k_sti
.global start_kthread
.global swap_kthread_regs

#.extern resume_irq0


k_cli:
    cli
    ret

k_sti:
    sti
    ret


##
# Starts a kernel thread.
# Updates esp and ebp, then calls the start function.
#
# Params:
#   8(%ebp)  - the new esp value
#   12(%ebp) - the new ebp value
#   16(%ebp) - the address of the starting function
#
start_kthread:
    
    pushl %ebp
    movl %esp, %ebp
    subl $8, %esp

    movl 8(%ebp), %eax
    movl %eax, -4(%ebp) # store esp

    movl 12(%ebp), %eax
    movl %eax, -8(%ebp) # store ebp

    movl 16(%ebp), %ecx # put the address of the start function in ecx

    movl -4(%ebp), %esp # update esp
    movl -8(%ebp), %ebp # update ebp

    sti                 # enable interrupts
    call *%ecx          # call the start function
    
    leave
    ret

##
# Populates the registers with the values from the next task.
# This procedure is only called while handling IRQ0, so after
# loading the register values, it jumps to IRQ0.
#
# Params:
#   8(%ebp)  - eflags
#   12(%ebp) - edi
#   16(%ebp) - esi
#   20(%ebp) - ebp
#   24(%ebp) - esp
#   28(%ebp) - ebx
#   32(%ebp) - edx
#   36(%ebp) - ecx
#   40(%ebp) - eax
#
swap_kthread_regs:

	movl %esp, %ebp
    subl $12, %esp

    movl 8(%ebp), %eax
    movl %eax, -4(%ebp)  # temporarily store eflags

    movl 20(%ebp), %eax
    movl %eax, -8(%ebp)  # temporarily store ebp

    movl 24(%ebp), %eax
    movl %eax, -12(%ebp) # temporarily store esp

    movl 12(%ebp), %edi  # update edi
    movl 16(%ebp), %esi  # update esi

    movl 28(%ebp), %ebx  # update ebx
    movl 32(%ebp), %edx  # update edx
    movl 36(%ebp), %ecx  # update ecx
    movl 40(%ebp), %eax  # update eax

    pushl -4(%ebp)
    popfl                # update eflags

    movl -12(%ebp), %esp # update esp
    movl -8(%ebp), %ebp  # update ebp

    ret
    #jmp resume_irq0      # resume IRQ0
