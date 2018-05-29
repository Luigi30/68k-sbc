/* Low-level keyboard handler. */
/* Should be used by the master input handler, not by applications directly. */

#ifndef DEV_KEYBOARD_H
#define DEV_KEYBOARD_H

#include <stdint.h>

#include "devices/devices.h"
#include "memmgr.h"
#include "nodelist.h"

#define DEVNAME_KEYBOARD "keyboard.device"

typedef struct {
  uint8_t keycode;
} KBD_KeyEvent;

typedef struct {
  DEVICE_Device device;
} DEVICE_Keyboard;

List *KBD_InputQueue;

void KBD_DeviceOpen();
void KBD_DeviceClose();
void KBD_InterruptHandler();

#endif
