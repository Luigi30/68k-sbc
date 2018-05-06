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
  DRAW_ModeReplace = 0x00,
  DRAW_ModeAND = 0x08,
  DRAW_ModeOR = 0x10,
  DRAW_ModeXOR = 0x18
} DRAW_LogicalMode;

static uint8_t pen_fore; // Foreground pen color.
static uint8_t pen_back; // Background pen color.

void DRAW_SetPenFore(uint8_t color_index);
void DRAW_SetPenBack(uint8_t color_index);

void DRAW_SetLogicalMode(DRAW_LogicalMode);

void DRAW_SetRectangle(Rectangle *r, uint16_t, uint16_t, uint16_t, uint16_t);
void DRAW_DrawRectangle(Rectangle *r);


#endif
