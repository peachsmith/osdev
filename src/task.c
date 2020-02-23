#include "kernel/task.h"
#include "kernel/pit.h"
#include "kernel/memory.h"
#include "kernel/vga.h"

#include "libc/stdlib.h"
#include "libc/stdio.h"
#include "libc/string.h"

// The initial starting point of the kernel stack
extern uint32_t init_esp;

// The currently running task
k_task* current_task = NULL;

k_task main_task;
k_task task_a;
k_task task_b;

extern void k_cli();
extern void k_sti();

extern void start_kthread(uint32_t esp, uint32_t ebp, void (*start)());

extern void swap_kthread_regs(uint32_t eflags,
	uint32_t edi,
	uint32_t esi,
	uint32_t ebp,
	uint32_t esp,
	uint32_t ebx,
	uint32_t edx,
	uint32_t ecx,
	uint32_t eax);

void* stack_a = NULL;
void* stack_b = NULL;

void task_a_action()
{
    for(;;)
    {
        printf("This is task a.\n");
    }
}

void task_b_action()
{
    for(;;)
    {
        printf("This is task b.\n");
    }
}

void debug_hang()
{
    printf("This is a debug hang.\n");
    for (;;);
}

void k_move_stack(void* new_start, uint32_t size)
{
    uint32_t i;       // index
    uint32_t old_esp; // old stack pointer
    uint32_t old_ebp; // old base pointer
    uint32_t offset;  // difference of old and new stack starting points
    uint32_t new_esp; // new stack pointer
    uint32_t new_ebp; // new base pointer

    for (i = (uint32_t)new_start; i >= (uint32_t)new_start - size; i -= 0x1000)
    {

    }

    // Read the current values of esp and ebp
    __asm__ volatile("mov %%esp, %0" : "=r" (old_esp));
    __asm__ volatile("mov %%ebp, %0" : "=r" (old_ebp));

    // Calculate the offset
    offset = (uint32_t)new_start - init_esp;

    // Calculate the new esp and ebp
    new_esp = old_esp + offset;
    new_ebp = old_ebp + offset;

    // Copy old stack contents into new stack
    memcpy((void*)new_esp, (void*)old_esp, init_esp - old_esp);

    for (i = (uint32_t)new_start; i > (uint32_t)new_start - size; i -= 4)
    {
        uint32_t tmp = *(uint32_t*)i;

        if (old_esp < tmp && tmp < init_esp)
        {
            tmp += offset;
            uint32_t* tmp2 = (uint32_t*)i;
            *tmp2 = tmp;
        }
    }

    __asm__ volatile("mov %0, %%esp" : : "r" (new_esp));
    __asm__ volatile("mov %0, %%ebp" : : "r" (new_ebp));
}

void k_init_tasking(uint32_t esp)
{
    k_cli();

    main_task.id = 1;
    main_task.status = JEP_TASK_RUNNING;
    main_task.esp = esp;
    main_task.ebp = 0;
    main_task.next = NULL;
    main_task.start = NULL;

    if (stack_a == NULL)
    {
        stack_a = k_palloc();
    }

    task_a.id = 2;
    task_a.status = JEP_TASK_NEW;
    task_a.esp = (uint32_t)stack_a + JEP_FRAME_LEN;
    task_a.ebp = (uint32_t)stack_a + JEP_FRAME_LEN;
    task_a.next = NULL;
    task_a.start = task_a_action;

    if (stack_b == NULL)
    {
        stack_b = k_palloc();
    }

    task_b.id = 3;
    task_b.status = JEP_TASK_NEW;
    task_b.esp = (uint32_t)stack_b + JEP_FRAME_LEN;
    task_b.ebp = (uint32_t)stack_b + JEP_FRAME_LEN;
    task_b.next = NULL;
    task_b.start = task_b_action;

    current_task = &main_task;

    k_sti();
}

void k_switch_task(uint32_t eflags,
	uint32_t edi,
	uint32_t esi,
	uint32_t ebp,
	uint32_t esp,
	uint32_t ebx,
	uint32_t edx,
	uint32_t ecx,
	uint32_t eax)
{
    // If there is no current task,
    // then tasking has not yet been initialized.
    if (current_task == NULL)
        return;
        
    // Update the stored CPU state of the current task.
    current_task->eflags = eflags;
    current_task->edi = edi;
    current_task->esi = esi;
    current_task->ebp = ebp;
    current_task->esp = esp;
    current_task->ebx = ebx;
    current_task->edx = edx;
    current_task->ecx = ecx;
    current_task->eax = eax;

    // Alternate between task a and task b.
    switch (current_task->id)
    {
        case 2:
            current_task = &task_b;
            break;

        case 1:
        case 3:
            current_task = &task_a;
            break;

        default:
            break;
    }

    // If the current task is new, start it.
    if (current_task->status == JEP_TASK_NEW)
    {
        current_task->status = JEP_TASK_RUNNING;

        start_kthread(
            current_task->esp,
            current_task->ebp,
            current_task->start
        );

        return;
    }

    // Swap the registers.
    swap_kthread_regs(
        current_task->eflags,
        current_task->edi,
        current_task->esi,
        current_task->ebp,
        current_task->esp,
        current_task->ebx,
        current_task->edx,
        current_task->ecx,
        current_task->eax
    );
}

void task_debug()
{
    fprintf(stddbg, "resuming IRQ0 from task switch\n");
}
