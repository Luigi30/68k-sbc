#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#include "memmgr.h"
#include "nodelist.h"

typedef struct Task Task;

typedef struct {
  uint8_t *name;
  List *message_queue;
} IPC_MessagePort;

typedef struct {
  Node node;                 // so we can enqueue messages
  IPC_MessagePort *reply_to; // set this if we need to reply to this message
  uint16_t length;           // length of this message, including header. set with sizeof(derived)
} IPC_Message;

IPC_Message *IPC_CreateMessage(uint16_t _length, IPC_MessagePort *_reply_to);
void IPC_SendMessage(IPC_Message *message, IPC_MessagePort *destination);
IPC_Message * IPC_GetMessage();

#endif
