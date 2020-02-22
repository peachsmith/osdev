.section .text

.global switch_to_task

.extern resume_irq0

.global start_task
#.global start_task

#.extern task_a_action
#.extern task_b_action

start_task:
    movl %esp, %ebp
    subl $8, %esp

    movl 8(%ebp), %eax
    movl %eax, -4(%ebp) # store esp

    movl 12(%ebp), %eax
    movl %eax, -8(%ebp) # store ebp

    movl 16(%ebp), %ecx

    movl -4(%ebp), %esp
    movl -8(%ebp), %ebp

    sti

    call *%ecx

switch_to_task:

	movl %esp, %ebp
    subl $12, %esp

	movl 8(%ebp), %eax
    movl %eax, -4(%ebp) # store eflags

    movl 20(%ebp), %eax
    movl %eax, -8(%ebp) # store ebp

    movl 24(%ebp), %eax
    movl %eax, -12(%ebp) # store esp

    movl 12(%ebp), %edi # update edi
    movl 16(%ebp), %esi # update esi

    movl 28(%ebp), %ebx # update ebx
    movl 32(%ebp), %edx # update edx
    movl 36(%ebp), %ecx # update ecx
    movl 40(%ebp), %eax # update eax

    # Attempt to update eflags
    # This currently causes a debug exception
    # pushl %eax
    # movl -4(%ebp), %eax
    # pushl %eax
    # popfl
    # popl %eax

    movl -12(%ebp), %esp # update esp
    movl -8(%ebp), %ebp  # update ebp

	#sti # enable interrupts
	ret # finish the task switch
