#include "messages.h"

#include "tasks/tasks.h"

IPC_Message * IPC_CreateMessage(uint16_t _length, IPC_MessagePort *_reply_to)
{
  IPC_Message *msg = MEMMGR_NewPtr(_length, H_SYSHEAP); // TODO: app heap?

  msg->length = _length;
  msg->reply_to = _reply_to;

  return msg; // Caller needs to fill in the struct they have created.
}

void IPC_SendMessage(IPC_Message *message, IPC_MessagePort *destination)
{
  LIST_AddTail(destination->message_queue, message);
}

IPC_Message * IPC_GetMessage()
{
  // Pop the first message in the calling task's queue.
  Task *task = TASK_GetRunningTask();
  task->info->signals.received &= ~SIG_MESSAGE;
  return (IPC_Message *)LIST_RemHead(task->info->message_port.message_queue);
}