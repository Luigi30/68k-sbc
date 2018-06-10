#include "devices/keyboard.h"
#include "devices/serial.h"

List KBD_InputQueue;

DEVICE_CommandFunctions DEVICE_KeyboardCommands[2];

extern void *RECV_BUFFER_VECTOR;

const uint8_t DEVNAME_KEYBOARD[] = "keyboard.device";

DEVICE_Keyboard *DEVICE_Keyboard_Create()
{
  DEVICE_Keyboard *kbd = MEMMGR_NewPtr(sizeof(DEVICE_Keyboard), H_SYSHEAP);
  kbd->device.logical_unit_number = 0;
  kbd->device.functions = DEVICE_KeyboardCommands;

  List *queue = MEMMGR_NewPtr(sizeof(List), 0);
  LIST_Init(queue, 0);
  kbd->device.message_port.message_queue = queue;
  printf("kbd->device.message_port.message_queue = %06X\n", &kbd->device.message_port.message_queue);

  kbd->device.name = DEVNAME_KEYBOARD;
  
  DEVICE_KeyboardCommands[0] = KBD_DeviceOpen;
  DEVICE_KeyboardCommands[1] = KBD_DeviceClose;
  
  kbd->device.is_open = 0;
  LIST_AddHead(&DEVICE_ActiveDevs, kbd);

  //TODO: install keyboard interrupt handler that places keypresses in a queue
  RECV_BUFFER_VECTOR = (void *)KBD_InterruptHandler;

  kbd->device.task = MEMMGR_NewPtr(sizeof(Task)+8, H_SYSHEAP);
  TaskInfo *kbd_taskinfo = MEMMGR_NewPtr(sizeof(TaskInfo), H_SYSHEAP);
  kbd->device.task->info = kbd_taskinfo;
  kbd->device.task->info->heap = TASK_SYSHEAP;
  TASK_Add(kbd->device.task, KBD_Task, 0, 4096);
  
  printf("DEVICE: Created device: %s. Device is at %06X\n", kbd->device.name, kbd);

  return kbd;
}

void KBD_DeviceOpen()
{
  LIST_Init(&KBD_InputQueue, 0);
  printf("KBD: device opened \n");
}

void KBD_DeviceClose()
{
  printf("KBD: TODO: hook up close for keyboard.device");
}

void KBD_InterruptHandler()
{
  // A keypress occurred and is in serial_in.
  DEVICE_Device *kbd = DEVICE_FindDeviceByName("keyboard.device");
  KBD_KeyEvent *msg = IPC_CreateMessage(sizeof(KBD_KeyEvent), NULL);
  msg->keycode = serial_in();

  // TODO: shouldn't IPC_SendMessage inform the receiving task?
  IPC_SendMessage(msg, &(kbd->message_port));
  SIGNAL_Send(DEVICE_FindDeviceByName("keyboard.device")->task, SIG_MESSAGE);
}

void KBD_Task()
{
  while(true)
  {
    printf("* KBD_Task *\n");

    IPC_Message *msg = IPC_GetMessage();
    printf("%c\n", ((KBD_KeyEvent *)msg)->keycode);

    //TODO: the task scheduler doesn't freeze this task here
    TASK_WaitForMessage();
  }  
}
