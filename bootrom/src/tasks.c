#include "tasks.h"

uint32_t ExecutingTaskRegisters[16];
uint16_t SFRAME_SR = 0;
uint32_t SFRAME_PC = 0;

uint8_t TASK_SwitchingEnabled = 0;

List *TASK_List;
List *TASK_ReadyList;

void TASK_InitSubsystem()
{
  TASK_List = MEMMGR_NewPtr(sizeof(List), H_SYSHEAP);
  LIST_Init(TASK_List, 0);

  TASK_ReadyList = MEMMGR_NewPtr(sizeof(List), H_SYSHEAP);
  LIST_Init(TASK_ReadyList, 0);
  
  //printf("List at %x is initialized\n", TASK_List);
  MMIO8(0x600017) = 0x40; // MFP vectors now begin at number 64
  MMIO8(0x600007) = 0x20; // enable TIMER A interrupt
  
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
  task->state = TASK_READY;
  
  //TODO: create a new heap for applications that need one  
  if(task->heap == TASK_SYSHEAP)
	task->stack_low = MEMMGR_NewPtr(stack_size, H_SYSHEAP);
  else if(task->heap == TASK_APPHEAP)
	task->stack_low = MEMMGR_NewPtr(stack_size, H_CURHEAP);
  else
	{
	  printf("TASK_Add: Heap selection not recognized!\nLocking system.\n");
	  while(true) {};
	}

  task->pc = (uint32_t)start_address;
  task->status_register = 0x2300; // TODO

  task->stack_high = (CPTR)((uint32_t)task->stack_low + stack_size);
  task->stack_pointer = task->stack_high;
  
  LIST_AddHead(TASK_List, (Node *)task);
  LIST_AddHead(TASK_ReadyList, (Node *)task);
  printf("TASK: Task %x added to task list. Task's SP is %x\n",
		 task,
		 task->stack_pointer);

  //MEMMGR_DumpHeapBlocks(&heap_system);
  //LIST_AddHead(&TASK_ReadyList, (Node *)task);
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
	  if(task->state != TASK_READY)
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
  //Find the running task.
  Task *current_task = TASK_GetRunningTask();
  if(current_task == NULL)
	{
	  //printf("Current task is NULL\n");
	}
  else
	{
	  //printf("Current task is %x\n", current_task);
	  // Back up the current registers to the Task struct.
	  for(int i=0;i<16;i++)
		{
		  current_task->registers[i] = ExecutingTaskRegisters[i];
		}
	  current_task->stack_pointer = (CPTR)ExecutingTaskRegisters[7];
	  current_task->pc = SFRAME_PC;
	  current_task->status_register = SFRAME_SR;
	}

  if(new_task == NULL)
	{
	  new_task = TASK_List->lh_Head;
	}

  TASK_RestoreRegisters(new_task);
  current_task->state = TASK_READY;
  new_task->state = TASK_RUNNING;
}

void TASK_RestoreRegisters(Task *new_task)
{
  // Restore the new task's registers.

  for(int i=0;i<16;i++)
	ExecutingTaskRegisters[i] = new_task->registers[i];
  
  //printf("New task is %x, new A7 is $%x\n", new_task, new_task->stack_pointer);
  
  ExecutingTaskRegisters[7] = (uint32_t)new_task->stack_pointer;
  SFRAME_PC = new_task->pc;
  SFRAME_SR = new_task->status_register;
}

Task *TASK_GetRunningTask()
{
  Task *task;
  for(task = TASK_List->lh_Head; task->node.ln_Succ != NULL; task = task->node.ln_Succ)
	{
	  if(task->state == TASK_RUNNING)
		return task;
	}

	return NULL; // uh... nothing is running
}

void TASK_ForbidInterrupts()
{
  // Disable the timer interrupt during *sensitive* operations.
  MMIO8(0x600013) = 0x00;
}

void TASK_AllowInterrupts()
{
  // Enable the timer interrupt, allowing timeslices to be processed.
  MMIO8(0x600013) = 0x20;
}

Task *TASK_FindReadyTask()
{
  Task *task;
  
  for(task = TASK_List->lh_Head; task->node.ln_Succ != NULL; task = task->node.ln_Succ)
	{
	  if(task->state == TASK_READY)
		return task;
	}

  // Nothing else is ready, so just load the current task again.
  //printf("*** No tasks ready ***\n");

  return NULL;
}
