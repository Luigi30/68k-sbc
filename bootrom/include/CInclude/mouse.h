#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>
#include <stdio.h>

#include "procyon.h"
#include "comport.h"

typedef struct {
  uint8_t buttons;
  uint8_t x;
  uint8_t y;
} MOUSE_Packet;

extern uint8_t mouse_packets_in_queue;
extern MOUSE_Packet mouse_packet_queue[16];
extern uint8_t mouse_position_x, mouse_position_y;

void MOUSE_SetupCOM1();
void MOUSE_PacketISR();

#endif
