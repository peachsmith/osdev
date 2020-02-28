.section .text

.global k_cli
.global k_sti
.global start_kthread


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
