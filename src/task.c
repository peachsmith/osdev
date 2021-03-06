#include "kernel/task.h"
#include "kernel/pit.h"
#include "kernel/memory.h"
#include "kernel/vga.h"

#include "libc/stdlib.h"
#include "libc/stdio.h"
#include "libc/string.h"

static uint32_t initialized = 0;

// The initial starting point of the kernel stack
extern uint32_t init_esp;

// The currently running task
k_task* current_task = NULL;

k_task main_task;
k_task task_a;
k_task task_b;

k_regs main_regs;
k_regs* regs_a;
k_regs* regs_b;

extern void k_cli();
extern void k_sti();

void* stack_a = NULL;
void* stack_b = NULL;

void task_a_action()
{
    uint32_t local_a = 4;
    for(;;)
    {
        printf("This is task a. Local variable: %d\n", local_a);
    }
}

void task_b_action()
{
    uint32_t local_b = 7;
    uint32_t tmp = 0;
    for(;;)
    {
        tmp++;
        if (tmp > 2000000)
        {
            printf("resetting tmp for task b\n");
            tmp = 0;
        }
        if (tmp < 10000)
            printf("This is task b. Local variable: %d, tmp: %d\n", local_b, tmp);
        else if (tmp < 20000)
            printf("The temporary variable in task b has exceeded 10000.\n");
        else
            printf("The temporary variable in task b has exceeded 20000.\n");
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

void k_init_tasking()
{
    k_cli();

    main_regs.esp = 0;
    main_regs.ebp = 0;

    main_task.id = 1;
    main_task.status = JEP_TASK_RUNNING;
    main_task.cpu_state = &main_regs;
    main_task.next = NULL;
    main_task.start = NULL;



    if (stack_a == NULL)
    {
        stack_a = k_palloc();
    }

    // Create the initial CPU state for a new kernel thread.
    regs_a = (k_regs*)((uint32_t)stack_a + JEP_FRAME_LEN - sizeof(k_regs));
    regs_a->ebp = (uint32_t)stack_a + JEP_FRAME_LEN;
    regs_a->eip = (uint32_t)task_a_action;
    regs_a->cs = 0x08;      // kernel code segment
    regs_a->eflags = 0x200; // default eflags value

    task_a.id = 2;
    task_a.status = JEP_TASK_NEW;
    task_a.cpu_state = regs_a;
    task_a.next = NULL;
    task_a.start = task_a_action;



    if (stack_b == NULL)
    {
        stack_b = k_palloc();
    }

    // Create the initial CPU state for a new kernel thread.
    regs_b = (k_regs*)((uint32_t)stack_b + JEP_FRAME_LEN - sizeof(k_regs));
    regs_b->ebp = (uint32_t)stack_b + JEP_FRAME_LEN;
    regs_b->eip = (uint32_t)task_b_action;
    regs_b->cs = 0x08;      // kernel code segment
    regs_b->eflags = 0x200; // default eflags value

    task_b.id = 3;
    task_b.status = JEP_TASK_NEW;
    task_b.cpu_state = regs_b;
    task_b.next = NULL;
    task_b.start = task_b_action;

    current_task = &main_task;

    initialized = 1;
    fprintf(stddbg, "tasking has now supposedly been initialized\n");

    k_sti();
}

uint32_t k_switch_task(uint32_t main_ticks, uint32_t esp)
{

    // If there is no current task,
    // then tasking has not yet been initialized.
    if (current_task == NULL || initialized == 0)
    {
        fprintf(stddbg, "tasking has not yet been initialized\n");
        return 0;
    }

    // Update the stored CPU state of the current task.
    current_task->cpu_state = (k_regs*)esp;

    // We currently switch tasks approximately every 2 seconds.
    // This logic is just for developing the task switch functionality.
    if (main_ticks % 2000 != 0)
        return (uint32_t)(current_task->cpu_state);

    // Alternate between tasks.
    switch (current_task->id)
    {
        case 1:
            current_task = &task_a;
            break;

        case 2:
            current_task = &task_b;
            break;

        case 3:
            current_task = &main_task;
            break;

        default:
            break;
    }

    return (uint32_t)(current_task->cpu_state);
}

void task_debug()
{
    fprintf(stddbg, "We should be here.\n");
}
