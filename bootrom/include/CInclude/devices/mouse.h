#ifndef DEVICES_MOUSE_H
#define DEVICES_MOUSE_H

#include <memmgr.h>
#include <stdint.h>

#include "devices/devices.h"
#include "nodelist.h"

typedef struct {
  DEVICE_Device device;
} DEVICE_Mouse;

DEVICE_Mouse *DEVICE_Mouse_Create();

void DEVICE_Mouse_DoSomething();

#endif
