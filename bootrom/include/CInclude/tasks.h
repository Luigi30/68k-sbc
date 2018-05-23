#ifndef TASKS_H
#define TASKS_H

#include "memmgr.h"
#include "nodelist.h"

extern uint32_t ExecutingTaskRegisters[16];
extern uint32_t NewTaskRegisters[16];

typedef enum {
  TASK_RUNNING, // task is currently running
  TASK_READY,   // task is ready to execute
  TASK_WAITING, // task is not ready to execute
  TASK_ADDED,   // just added to task list
  TASK_REMOVED, // just removed from task list list
} TASK_State;

typedef enum {
  TASK_SIGABORT,
} TASK_Signal;

typedef enum {
  TASK_SYSHEAP, // task allocates memory in SysHeap
  TASK_APPHEAP,  // task allocates memory in AppHeap
} TASK_Heap;

typedef struct {
  Node node;
  TASK_State state;
  TASK_Heap heap;  // which heap are we using?
  CPTR stack_low;  // the bottom of the application's stack
  CPTR stack_high; // the top of the application's stack
  CPTR stack_pointer; // the application's SP at last execution
  CPTR entry_point;
  
  uint32_t registers[16];
  uint16_t status_register;
  uint32_t pc;

  Heap *task_heap;
} Task;

extern List *TASK_List;
extern List *TASK_ReadyList;

void TASK_InitSubsystem();
void TASK_EnableSwitching(uint8_t enable);

void TASK_UpdateReadyQueue();
void TASK_ProcessQuantum();

void TASK_Add(Task *task, CPTR start_address, CPTR cleanup_address, uint32_t stack_size);
void TASK_ContextSwitch(Task *new_task);
void TASK_RestoreRegisters(Task *new_task);

struct Task * TASK_FindReadyTask();
struct Task * TASK_GetRunningTask();

extern void TASK_SwitchToNextReadyTask();

#endif
