
#ifndef DEVICES_DEVICE_H
#define DEVICES_DEVICE_H

/* 
   Device handlers!

   Devices are a standard method of interacting with different I/O subsystems.

   All devices support a certain set of commands, with extended commands depending
   on the actual device in question.
*/

struct {
  
} DEVICE_CommandQueue;

struct {
  uint8_t device_name[16];
  uint8_t logical_unit_number;
} DEVICE_Generic;

#endif
