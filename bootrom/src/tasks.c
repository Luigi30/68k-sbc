#include "tasks.h"

uint32_t ExecutingTaskRegisters[16];
uint16_t SFRAME_SR = 0;
uint32_t SFRAME_PC = 0;

uint8_t TASK_SwitchingEnabled = 0;

List *TASK_List;
List *TASK_ReadyList;

extern void LIST_AddHead(__reg("a0") List *list, __reg("a1") Node *node);
extern void LIST_AddTail(__reg("a0") List *list, __reg("a1") Node *node);

extern Node *LIST_RemHead(__reg("a0") List *list);
extern Node *LIST_RemTail(__reg("a0") List *list);
extern Node *LIST_Remove(__reg("a0") List *list, __reg("a1") Node *node);

Task *running_task = NULL;

void TASK_InitSubsystem()
{
  TASK_List = MEMMGR_NewPtr(sizeof(List), H_SYSHEAP);
  LIST_Init(TASK_List, 0);

  TASK_ReadyList = MEMMGR_NewPtr(sizeof(List), H_SYSHEAP);
  LIST_Init(TASK_ReadyList, 0);
  
  //printf("List at %x is initialized\n", TASK_List);
  MMIO8(0x600017) = 0x40; // MFP vectors now begin at number 64
  MMIO8(0x600007) = MMIO8(0x600007) | 0x20; // enable TIMER A interrupt
  
  // Set TIMER A for delay mode, 200 prescaler
  MMIO8(0x600019) = 0x10 | 0x07;
}

void TASK_EnableSwitching(uint8_t enable)
{
  TASK_SwitchingEnabled = enable;
}

void TASK_Add(Task *task,
			  CPTR start_address,   // the task's entry point
			  CPTR cleanup_address, // if the task returns, call this
			  uint32_t stack_size)  // bytes to allocate for the stack
{
  //printf("task == %6X\n", task);
  //printf("task->info == %6X\n", task->info);
  //printf("status register addr is %6X\n", &(task->info->status_register));
  task->info->state = TASK_READY;
  
  //TODO: create a new heap for applications that need one  
  if(task->info->heap == TASK_SYSHEAP)
	task->info->stack_low = MEMMGR_NewPtr(stack_size, H_SYSHEAP);
  else if(task->info->heap == TASK_APPHEAP)
	task->info->stack_low = MEMMGR_NewPtr(stack_size, H_CURHEAP);
  else
	{
	  printf("TASK_Add: Heap selection not recognized!\nLocking system.\n");
	  while(true) {};
	}


  task->info->pc = (uint32_t)start_address;
  task->info->status_register = 0x2300; // TODO

  task->info->stack_high = (CPTR)((uint32_t)task->info->stack_low + stack_size - 8);
  task->info->stack_pointer = task->info->stack_high;
  
  LIST_AddTail(TASK_List, (Node *)task);
  LIST_AddTail(TASK_ReadyList, (Node *)task);
  printf("TASK: Task %x added to task list. Task's SP is %x\n",
		 task,
		 task->info->stack_pointer);

  printf("*** TASK_List ***\n");
  printf("Head: %6X, Tail: %6X\n", TASK_ReadyList->lh_Head, TASK_ReadyList->lh_Tail);

  //MEMMGR_DumpHeapBlocks(&heap_system);
}

void TASK_Wait(Task *task)
{
  // Put a task to sleep until it receives a signal.  
}

void TASK_UpdateReadyQueue()
{
  Task *task;

  for(task = TASK_ReadyList->lh_Head; task->node.ln_Succ != NULL; task = task->node.ln_Succ)
	{
	  if(task->info->state != TASK_READY)
		LIST_Remove(TASK_ReadyList, task);
	}

  //TODO: look for ready tasks and add them to the queue
}

void TASK_ProcessQuantum()
{
  // The timer expired. See if we need to switch to another task.

  // The scheduler is disabled, don't do anything.
  if(TASK_SwitchingEnabled == 0)
	{
	  return;
	}
  
  //printf("Timeslice expired!\n");

  TASK_ContextSwitch(TASK_FindReadyTask());
}

void TASK_ContextSwitch(Task *new_task)
{
  //printf("*** Context Switch to Task %06X ***\n", new_task);

  //Find the running task.
  //  Task *current_task = TASK_GetRunningTask();
  Task *current_task = running_task;
  if(current_task == NULL)
	{
	  printf("Current task is NULL\n");
	}
  else
	{
	  //printf("Current task is %x\n", current_task);
	  // Back up the current registers to the Task struct.
	  for(int i=0;i<16;i++)
		{
		  current_task->info->registers[i] = ExecutingTaskRegisters[i];
		}
	  current_task->info->stack_pointer = (CPTR)ExecutingTaskRegisters[7];
	  current_task->info->pc = SFRAME_PC;
	  current_task->info->status_register = SFRAME_SR;

	  //printf("Adding task %x to ReadyList\n", current_task);
	  LIST_AddTail(TASK_ReadyList, current_task);
	  current_task->info->state = TASK_READY;
	}

  TASK_RestoreRegisters(new_task);
  
  new_task->info->state = TASK_RUNNING;

  // Re-queue the current task.
  LIST_Remove(TASK_ReadyList, new_task);
  running_task = new_task;  
}

void TASK_RestoreRegisters(Task *new_task)
{
  // Restore the new task's registers.

  for(int i=0;i<16;i++)
	ExecutingTaskRegisters[i] = new_task->info->registers[i];
  
  //printf("New task is %x, new A7 is $%x\n", new_task, new_task->info->stack_pointer);
  
  ExecutingTaskRegisters[7] = (uint32_t)new_task->info->stack_pointer;
  SFRAME_PC = new_task->info->pc;
  SFRAME_SR = new_task->info->status_register;
}

Task *TASK_GetRunningTask()
{
  Task *task;
  for(task = TASK_List->lh_Head; task->node.ln_Succ != NULL; task = task->node.ln_Succ)
	{
	  printf("GetRunningTask: Examining task %6X. Task status is %d\n",
			 task,
			 task->info->state);
	  if(task->info->state == TASK_RUNNING)
		return task;
	}

  printf("Nothing is running?\n");
  return NULL; // uh... nothing is running
}

void TASK_ForbidInterrupts()
{
  // Disable the timer interrupt during *sensitive* operations.
  MMIO8(0x600013) = MMIO8(0x600013) & 0xDF;
}

void TASK_AllowInterrupts()
{
  // Enable the timer interrupt, allowing timeslices to be processed.
  MMIO8(0x600013) = MMIO8(0x600013) | 0x20;
}

Task *TASK_FindReadyTask()
{
  Task *task;

  for(task = TASK_ReadyList->lh_Head; task->node.ln_Succ != NULL; task = task->node.ln_Succ)
	{
	  //printf("FindReadyTask: Examining task $%6X. State is %d\n", task, task->info->state);
	  if(task->info->state == TASK_READY)
		return task;
	}

  // Nothing else is ready, so just load the current task again.
  //printf("* No tasks are ready, returning running task\n");
  return TASK_GetRunningTask();
}
