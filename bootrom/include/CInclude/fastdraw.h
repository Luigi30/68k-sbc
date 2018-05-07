#ifndef FASTDRAW_H
#define FASTDRAW_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include "procyon.h"
#include "vga.h"

typedef struct point_t
{
  uint16_t x, y;
} Point;

typedef struct rectangle_t
{
  uint16_t x, y, width, height;
} Rectangle;

typedef enum {
  DEPTH_CHUNKY = -2,
  DEPTH_TEXT = -1,
  DEPTH_1BPP = 1,
  DEPTH_2BPP = 2,
  DEPTH_4BPP = 4  
} DRAWPORT_DEPTH;

typedef struct drawport_t {
  uint8_t *vram_base;
  Point size;
  uint8_t depth;
} DrawPort;

static DrawPort screenPort; // TODO: can we move the base address of the VGA screen?
                            // If so, abolish this and make it dynamic.
typedef enum {
  DRAW_ModeReplace = 0x00,
  DRAW_ModeAND = 0x08,
  DRAW_ModeOR = 0x10,
  DRAW_ModeXOR = 0x18
} DRAW_LogicalMode;

static DrawPort *activeDrawPort;
static Point penLocation;
static uint8_t pen_fore; // Foreground pen color.
static uint8_t pen_back; // Background pen color.

void DRAW_Init();

void DRAW_MovePen(Point p); 
void DRAW_SetPenFore(uint8_t color_index);
void DRAW_SetPenBack(uint8_t color_index);

void DRAW_SetVGAMode(VGA_Mode mode);
void DRAW_SetLogicalMode(DRAW_LogicalMode);

void DRAW_SetRectangle(Rectangle *r, uint16_t, uint16_t, uint16_t, uint16_t);
void DRAW_DrawRectangle(Rectangle *r);


#endif
