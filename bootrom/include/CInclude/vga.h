#ifndef VGA_H
#define VGA_H

#include "procyon.h"
#include <stdint.h>
#include <stdio.h>

#define ISA_VGA_IOBASE 0xFA0000
#define ISA_VGA_MEMBASE 0x800000
#define VGA_IO_ADDRESS(ADDR) ( ISA_VGA_IOBASE + (2 * (ADDR) ) )
#define VGA_MEM_ADDRESS(ADDR) ( ISA_VGA_MEMBASE + (2 * (ADDR) ) )

typedef enum vga_mode_t {
  VGA_MODE_80x25 = 0x03,
  VGA_MODE_12h = 0x12,
  VGA_MODE_13h = 0x13,
  VGA_MODE_2Eh = 0x2E
} VGA_Mode;

typedef struct {
  uint16_t size_x, size_y;
} VGA_ModeInfo;
static VGA_ModeInfo VGA_MODEINFO;

typedef struct {
  uint8_t *data;     //pointer to start of bitmap in work RAM.
  uint16_t size_x;   //pixels
  uint16_t size_y;   //pixels
  uint8_t *vram_ptr; //pointer to start of bitmap in VRAM. if not in VRAM, null.
} VGA_Bitmap;

typedef struct {
  VGA_Bitmap bitmap;
  uint16_t glyph_x;
  uint16_t glyph_y;
} VGA_Font;

typedef enum {
  BITMAP_NONE = 0x0,
  BITMAP_FLIP_X = 0x1,
  BITMAP_FLIP_Y = 0x2
} BITMAP_FLAGS;

extern uint8_t BITMAP_PolluxVGA[];
extern uint8_t BITMAP_Fixedsys12[];
extern uint8_t BITMAP_8bitpusab12[];

uint8_t VGA_IsInVBLANK();
void VGA_SetControllerRegister(uint16_t, uint16_t, uint8_t, uint8_t);
void VGA_SetDMAColor(uint8_t, uint8_t, uint8_t);
void VGA_SetBitplaneWriteMask(uint8_t);

void VGA_SetWriteMode(int);

void VGA_SetMode(enum VGA_Mode);
void VGA_Set80x25Mode();
void VGA_SetMode12h();
void VGA_SetMode13h();
void VGA_SetMode2Eh();

extern void VGA_SetPixel(__reg("d0") uint16_t, __reg("d1") uint16_t, __reg("d2") uint8_t);
extern void VGA_SetMode12Pixel(__reg("d0") uint16_t, __reg("d1") uint16_t, __reg("d2") uint8_t);
void VGA_SetMode13Pixel(uint16_t, uint16_t, uint8_t);

void VGA_PutBitmap(VGA_Bitmap *, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, BITMAP_FLAGS);
				   
void VGA_SetStandardPalette();

void VGA_ChainPlanes();
void VGA_UnchainPlanes();

#endif
