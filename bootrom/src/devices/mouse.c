#include "devices/mouse.h"

DEVICE_CommandFunctions DEVICE_MouseCommands[] = { DEVICE_Mouse_DoSomething };

DEVICE_Mouse *DEVICE_Mouse_Create()
{
  DEVICE_Mouse *mouse = MEMMGR_NewPtr(sizeof(DEVICE_Mouse), H_SYSHEAP);
  mouse->device.logical_unit_number = 0;
  mouse->device.functions = DEVICE_MouseCommands;
  mouse->device.is_open = 0;

  mouse->device.name = MEMMGR_NewPtr(32, H_SYSHEAP);
  strcpy(mouse->device.name, "mouse.device");
  
  LIST_AddHead(&DEVICE_ActiveDevs, mouse);

  printf("DEVICE: Created device: %s. Device is at %06X\n", mouse->device.name, mouse);

  return mouse;
}

void DEVICE_Mouse_DoSomething()
{
  printf("The mouse device is executing a command without parameters.\n");
}

