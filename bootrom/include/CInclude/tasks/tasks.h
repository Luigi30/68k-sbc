#ifndef TASKS_H
#define TASKS_H

#include "memmgr.h"
#include "nodelist.h"
#include "tasks/signals.h"
#include "messages.h"

extern uint32_t ExecutingTaskRegisters[16];
extern uint32_t NewTaskRegisters[16];

typedef enum {
  TASK_RUNNING  = 0, // task is currently running
  TASK_READY    = 1,   // task is ready to execute
  TASK_WAITING  = 2, // task is not ready to execute
  TASK_ADDED    = 3,   // just added to task list
  TASK_REMOVED  = 4, // just removed from task list list
} TASK_State;

typedef enum {
  TASK_SIGABORT,
} TASK_Signal;

typedef enum {
  TASK_SYSHEAP, // task allocates memory in SysHeap
  TASK_APPHEAP,  // task allocates memory in AppHeap
} TASK_Heap;

typedef struct {
  TASK_State state;
  TASK_Heap heap;  // which heap are we using?
  CPTR stack_low;  // the bottom of the application's stack
  CPTR stack_high; // the top of the application's stack
  CPTR stack_pointer; // the application's SP at last execution
  CPTR entry_point;
  SIGNAL_Block signals;
  IPC_MessagePort message_port;

  uint16_t status_register;
  uint32_t pc;
  uint32_t registers[16];
  
} TaskInfo;

typedef struct Task {
  Node node;
  TaskInfo *info;
} Task;

extern List *TASK_List;
extern List *TASK_ReadyList;

void TASK_InitSubsystem();
void TASK_EnableSwitching(uint8_t enable);

void TASK_CleanReadyQueue();
void TASK_ProcessQuantum();

void TASK_Add(Task *task, CPTR start_address, CPTR cleanup_address, uint32_t stack_size);
void TASK_ContextSwitch(Task *new_task);
void TASK_RestoreRegisters(Task *new_task);

void TASK_WaitForMessage(); // implicit argument is the current active task

struct Task * TASK_FindReadyTask();
struct Task * TASK_GetRunningTask();

extern void TASK_SwitchToNextReadyTask();

#endif
