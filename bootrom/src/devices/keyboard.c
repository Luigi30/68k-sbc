#include "devices/keyboard.h"
#include "devices/serial.h"

List KBD_InputQueue;

DEVICE_CommandFunctions DEVICE_KeyboardCommands[2];

extern void *RECV_BUFFER_VECTOR;

DEVICE_Keyboard *DEVICE_Keyboard_Create()
{
  DEVICE_Keyboard *kbd = MEMMGR_NewPtr(sizeof(DEVICE_Keyboard), H_SYSHEAP);
  kbd->device.logical_unit_number = 0;
  kbd->device.functions = DEVICE_KeyboardCommands;

  kbd->device.message_port.message_queue = MEMMGR_NewPtr(sizeof(List), 0);
  LIST_Init(kbd->device.message_port.message_queue, 0);

  kbd->device.name = MEMMGR_NewPtr(32, H_SYSHEAP);
  strcpy(kbd->device.name, DEVNAME_KEYBOARD);
  
  DEVICE_KeyboardCommands[0] = KBD_DeviceOpen;
  DEVICE_KeyboardCommands[1] = KBD_DeviceClose;
  
  kbd->device.is_open = 0;
  LIST_AddHead(&DEVICE_ActiveDevs, kbd);

  //TODO: install keyboard interrupt handler that places keypresses in a queue
  RECV_BUFFER_VECTOR = (void *)KBD_InterruptHandler;
  
  printf("DEVICE: Created device: %s. Device is at %06X\n", kbd->device.name, kbd);

  return kbd;
}

void KBD_DeviceOpen()
{
  LIST_Init(&KBD_InputQueue, 0);
  printf("opened keyboard.device");
}

void KBD_DeviceClose()
{
  printf("TODO: hook up close for keyboard.device");
}

void KBD_InterruptHandler()
{
  //serial_string_out("Keypress! TODO: queue it\n");
}
