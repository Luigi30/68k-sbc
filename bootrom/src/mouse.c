#include "mouse.h"

uint8_t mouse_packets_in_queue;
MOUSE_Packet mouse_packet_queue[16];

uint8_t mouse_position_x = 100;
uint8_t mouse_position_y = 100;

void MOUSE_SetupCOM1()
{
  /* Setup COM1 to read an MS protocol mouse. */

  //1200bps
  MMIO16(ISA_IO_ADDRESS(COM1_LCR)) = 0x80;
  MMIO16(ISA_IO_ADDRESS(COM1_BAUD_LSB)) = 0x60;
  MMIO16(ISA_IO_ADDRESS(COM1_BAUD_MSB)) = 0x00;
  MMIO16(ISA_IO_ADDRESS(COM1_LCR)) = 0x00;

  //7-N-1
  MMIO16(ISA_IO_ADDRESS(COM1_LCR)) = 0x02;

  //FIFO on and cleared
  MMIO16(ISA_IO_ADDRESS(COM1_FIFO)) = 0x07;

  //Interrupt when data received
  MMIO16(ISA_IO_ADDRESS(COM1_INTEN)) = 0x01;

  //DTR and RTS enabled
  MMIO16(ISA_IO_ADDRESS(COM1_MCR)) = 0x03;
}

extern uint8_t MOUSE_IncomingPacket[3];
extern uint8_t MOUSE_PacketBytesReceived;

void MOUSE_PacketToStruct()
{ 
  MOUSE_Packet packet;
  packet.buttons = ((MOUSE_IncomingPacket[0] & 0x20) ? 0x01 : 0x00) |
	(MOUSE_IncomingPacket[0] & 0x10 ? 0x02 : 0x00);
	
  packet.x = MOUSE_IncomingPacket[1] | ((MOUSE_IncomingPacket[0] & 0x03) << 6);
  packet.y = MOUSE_IncomingPacket[2] | ((MOUSE_IncomingPacket[0] & 0x0C) << 4);

  printf("MOUSE INTERRUPT: BUTTONS: %x, MOVEMENT_X: %x, MOVEMENT_Y: %x\n",
		 packet.buttons, packet.x, packet.y);
}
