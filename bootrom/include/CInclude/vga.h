#ifndef VGA_H
#define VGA_H

#include "procyon.h"
#include <stdint.h>

#define ISA_VGA_IOBASE 0xFA0000
#define ISA_VGA_MEMBASE 0x800000
#define VGA_IO_ADDRESS(ADDR) ( ISA_VGA_IOBASE + (2 * (ADDR) ) )
#define VGA_MEM_ADDRESS(ADDR) ( ISA_VGA_MEMBASE + (2 * (ADDR) ) )

typedef enum vga_mode_t {
  VGA_MODE_80x25 = 0x03,
  VGA_MODE_12h = 0x12,
  VGA_MODE_13h = 0x13
} VGA_Mode;

extern uint8_t BITMAP_PolluxVGA[];

void VGA_SetControllerRegister(uint16_t, uint16_t, uint8_t, uint8_t);
void VGA_SetDMAColor(uint8_t, uint8_t, uint8_t);
void VGA_SetBitplaneWriteMask(uint8_t);

void VGA_SetWriteMode(int);

void VGA_SetMode(enum VGA_Mode);
void VGA_Set80x25Mode();
void VGA_SetMode12h();
void VGA_SetMode13h();

extern void VGA_SetPixel(__reg("d0") uint16_t, __reg("d1") uint16_t, __reg("d2") uint8_t);
extern void VGA_SetMode12Pixel(__reg("d0") uint16_t, __reg("d1") uint16_t, __reg("d2") uint8_t);
void VGA_SetMode13Pixel(uint16_t, uint16_t, uint8_t);

void VGA_PutBitmap(uint8_t *, uint16_t, uint16_t, uint16_t, uint16_t);
				   
void VGA_SetStandardPalette();

void VGA_ChainPlanes();
void VGA_UnchainPlanes();

#endif
