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

    uint32_t eflags;
    uint32_t cs;
    uint32_t eip;
    
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    
    struct k_task* next;
    void (*start)();
    
}__attribute__((packed))k_task;

void k_move_stack(void* new_start, uint32_t size);

void k_init_tasking();

void task_debug(uint32_t esp);

k_task* k_switch_task(uint32_t main_ticks,
    uint32_t real_esp,
	uint32_t edi,
	uint32_t esi,
	uint32_t ebp,
	uint32_t esp,
	uint32_t ebx,
	uint32_t edx,
	uint32_t ecx,
	uint32_t eax,
    uint32_t eip,
    uint32_t cs,
    uint32_t eflags
);

#endif
