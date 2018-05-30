#ifndef DEVICES_DEVICES_H
#define DEVICES_DEVICES_H

#include <stdint.h>

#include "messages.h"
#include "nodelist.h"

/* Commands that all devices must process. */
#define CMD_OPEN 0     // Add 1 to the Open count. Initialize the device if required.
#define CMD_CLOSE 1    // Subtract 1 from the Open count. If 0, cleanup and turn off the device.
#define CMD_READ 2     // Read something from the device.
#define CMD_WRITE 3    // Write something to the device.
#define CMD_RESET 4    // Reset the device to a known state.
#define CMD_INACTIVE 5 // The device will stop processing commands while inactive.
#define CMD_ACTIVE 6   // The device will continue processing commands if inactive. If active, no effect.

typedef void(*DEVICE_CommandFunctions)(void);

typedef struct {
  Node dev_node;
  DEVICE_CommandFunctions *functions;
  uint8_t *name;
  IPC_MessagePort message_port;
  uint8_t logical_unit_number;
  uint8_t is_open;
  uint8_t is_active;
} DEVICE_Device;

extern List DEVICE_ActiveDevs;

void DEVICE_InitSubsystem();
void DEVICE_PrintAllDevices();
void DEVICE_DoCommand(uint8_t *device_name, uint8_t command_index);

DEVICE_Device *DEVICE_FindDeviceByName(uint8_t *device_name);

#endif
