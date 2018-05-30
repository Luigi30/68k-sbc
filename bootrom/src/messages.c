#include "messages.h"

IPC_Message *IPC_CreateMessage(uint16_t _length, IPC_MessagePort *_reply_to)
{
  IPC_Message *msg = MEMMGR_NewPtr(_length, H_SYSHEAP); // TODO: app heap?

  msg->length = _length;
  msg->reply_to = _reply_to;

  return msg; // Caller needs to fill in the struct they have created.
}

void IPC_SendMessage(IPC_Message *message, IPC_MessagePort *destination)
{
  LIST_AddTail(destination->message_queue, message);
  printf("Message at %06X inserted into message queue at %06X\n", message, destination);
}
