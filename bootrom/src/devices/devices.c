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
	  
	  printf("DEVICE %06X: '%s' - IsOpen: %d\n", dev, dev->name, dev->is_open);
	}
}

DEVICE_Device *DEVICE_FindDeviceByName(uint8_t *device_name)
{
  DEVICE_Device *dev;
  for(dev = DEVICE_ActiveDevs.lh_Head; dev->dev_node.ln_Succ != NULL; dev = dev->dev_node.ln_Succ)
	{
	  if(strcmp(dev->name, device_name) == 0)
		{
		  return dev;
		}
	}
  return NULL;
}

void DEVICE_DoCommand(uint8_t *device_name, uint8_t command_index)
{
  /*
  DEVICE_Device *dev = (DEVICE_Device *)DEVICE_ActiveDevs.lh_Head;
  */
  DEVICE_Device *dev;
  for(dev = DEVICE_ActiveDevs.lh_Head; dev->dev_node.ln_Succ != NULL; dev = dev->dev_node.ln_Succ)
	{
	  if(strcmp(dev->name, device_name) == 0)
		{
		  break;
		}
	}
  
  //printf("DEVICE_DoCommand: execute command index %d on device %s.\n", command_index, device_name);
  //printf("Jumping to function at %06X. Command list is at %06X\n", dev->functions[0], dev->functions);
  dev->functions[0]();
}
