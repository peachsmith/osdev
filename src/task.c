#include "kernel/task.h"
#include "kernel/pit.h"

#include "libc/stdlib.h"
#include "libc/stdio.h"
#include "libc/string.h"

// The initial starting point of the kernel stack
extern uint32_t init_esp;

// Gets the value of the instruction pointer
extern uint32_t read_eip();

// TODO: implement better memory management for copying and moving
// stacks and virtual address spaces

// The currently running task
volatile k_task* current_task = NULL;

// The list of tasks
volatile k_task* task_queue = NULL;

volatile k_task task_1;
volatile k_task task_2;
volatile k_task task_3;

extern void switch_to_task(uint32_t eip, uint32_t esp, uint32_t ebp);

void task_2_action()
{
    //printf("This is task 2.\n");
    for(;;)
    {
        //k_pit_waits(1);
        //printf("This is still task 2.\n");
    }

    // for(;;)
    // {
    //     printf("This is task 2\n");
    // }
}

void task_3_action()
{
    //printf("This is task 3.\n");
    for(;;)
    {
        //k_pit_waits(1);
        //printf("This is still task 3.\n");
    }

    // for(;;)
    // {
    //     printf("This is task 3\n");
    // }
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
    __asm__ volatile("cli");

    // TODO: move the stack

    // Create the first task
    // task_queue = (k_task*)malloc(sizeof(k_task));

    // current_task = task_queue;
    // current_task->id = 1;
    // current_task->esp = 0;
    // current_task->ebp = 0;
    // current_task->eip = 0;
    // current_task->next = NULL;
    task_1.id = 1;
    task_1.status = JEP_TASK_RUNNING;
    task_1.esp = 0;
    task_1.ebp = 0;
    task_1.eip = 0;
    task_1.next = NULL;
    task_1.start = NULL;

    task_2.id = 2;
    task_2.status = JEP_TASK_NEW;
    task_2.esp = 0;
    task_2.ebp = 0;
    task_2.eip = 0;
    task_2.next = NULL;
    task_1.start = NULL;

    task_3.id = 3;
    task_3.status = JEP_TASK_NEW;
    task_3.esp = 0;
    task_3.ebp = 0;
    task_3.eip = 0;
    task_3.next = NULL;
    task_1.start = NULL;

    current_task = &task_1;

    __asm__ volatile("sti");
}

void k_switch_task()
{
    __asm__ volatile("cli");

    uint32_t esp; // stack pointer
    uint32_t ebp; // base pointer
    uint32_t eip; // instruction pointer

    // If there is no current task,
    // then tasking has not yet been initialized.
    if (current_task == NULL)
        return;

    // Read the current values of esp, ebp, and eip
    //__asm__ volatile("mov %%esp, %0" : "=r" (esp));
    //__asm__ volatile("mov %%ebp, %0" : "=r" (ebp));
    //eip = read_eip();
        
    // Update the esp, ebp, and eip of the current task
    current_task->esp = esp;
    current_task->ebp = ebp;
    current_task->eip = eip;

    // Change which task is the current task.
    // For now, we just alternate between two tasks
    switch (current_task->id)
    {
        case 1:
        case 2:
            printf("switching to task 3\n");
            current_task = &task_3;
            break;

        case 3:
            printf("switching to task 2\n");
            current_task = &task_2;
            break;

        default:
            break;
    }

    // Temporary code to initialize tasks
    if (current_task->status == JEP_TASK_NEW)
    {
        switch (current_task->id)
        {
        case 2:
            printf("initializing task 2\n");
            current_task->esp = (uint32_t)(task_2_action);
            current_task->ebp = (uint32_t)(task_2_action);
            //current_task->eip = (uint32_t)(task_2_action);
            break;

        case 3:
            printf("initializing task 3\n");
            current_task->esp = (uint32_t)(task_3_action);
            current_task->ebp = (uint32_t)(task_3_action);
            //current_task->eip = (uint32_t)(task_3_action);
            break;
        
        default:
            break;
        }

        current_task->status = JEP_TASK_RUNNING;
    }

    // Get the esp, ebp, and eip of the next task
    esp = current_task->esp;
    ebp = current_task->ebp;
    eip = current_task->eip;

    printf("id: [%3d] ebp: [%-#12X] esp: [%-#12X] eip [%-#12X]\n", current_task->id, ebp, esp, eip);

    switch_to_task(eip, esp, ebp);
}
