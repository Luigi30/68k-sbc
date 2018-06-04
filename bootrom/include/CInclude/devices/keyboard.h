/* Low-level keyboard handler. */
/* Should be used by the master input handler, not by applications directly. */

#ifndef DEV_KEYBOARD_H
#define DEV_KEYBOARD_H

#include <stdint.h>

#include "devices/devices.h"
#include "messages.h"
#include "memmgr.h"
#include "nodelist.h"

typedef struct {
  IPC_Message message;
  uint8_t keycode;
} KBD_KeyEvent;

typedef struct {
  DEVICE_Device device;
} DEVICE_Keyboard;

extern List KBD_InputQueue;

DEVICE_Keyboard *DEVICE_Keyboard_Create();

void KBD_DeviceOpen();
void KBD_DeviceClose();
void KBD_InterruptHandler();

void KBD_Task();

#endif
