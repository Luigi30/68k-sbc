#include "devices/devices.h"

List DEVICE_ActiveDevs;

void DEVICE_InitSubsystem()
{
  LIST_Init(&DEVICE_ActiveDevs, 0);
}

void DEVICE_PrintAllDevices()
{
  Node *current;
  
  printf("*** DEVICE LIST ***\n");
  for(current = DEVICE_ActiveDevs.lh_Head; current->ln_Succ != NULL; current = current->ln_Succ)
	{
	  DEVICE_Device *dev = (DEVICE_Device *)current;
	  
	  printf("DEVICE %06X: '%s' - IsOpen: %d\n", dev, dev->long_name, dev->is_open);
	}
}

void DEVICE_DoCommand(uint8_t *device_name, uint8_t command_index)
{
  //TODO: look up the device. and the command. lol.
  DEVICE_Device *dev = (DEVICE_Device *)DEVICE_ActiveDevs.lh_Head;

  printf("DEVICE_DoCommand: execute command index %d on device %s\n", command_index, device_name);
  dev->functions[0]();
}
