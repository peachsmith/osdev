#ifndef JEP_TASK_H
#define JEP_TASK_H

#include <stdint.h>

// A value to help with task switching.
// This probably isn't the best way to go about things.
#define JEP_TASK_MAGIC 0x1

#define JEP_TASK_NEW        0x1
#define JEP_TASK_RUNNING    0x2
#define JEP_TASK_WAITING    0x3
#define JEP_TASK_TERMINATED 0x4

typedef struct k_task{
    uint32_t id;
    uint32_t status;
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    struct k_task* next;
    void (*start)();
    
}__attribute__((packed))k_task;

void k_move_stack(void* new_start, uint32_t size);

void k_init_tasking();

void k_switch_task();

#endif
