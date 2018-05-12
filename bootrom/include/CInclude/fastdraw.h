#ifndef FASTDRAW_H
#define FASTDRAW_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "procyon.h"
#include "vga.h"

#include "fastdraw/types.h"
#include "fastdraw/drawport.h"

//static DrawPort *activeDrawPort;
extern Point penLocation;
extern uint8_t pen_fore; // Foreground pen color.
extern uint8_t pen_back; // Background pen color.

void DRAW_Init();

void DRAW_MovePen(Point p); 
void DRAW_SetPenFore(uint8_t color_index);
void DRAW_SetPenBack(uint8_t color_index);

void DRAW_SetVGAMode(VGA_Mode mode);
void DRAW_SetLogicalMode(DRAW_LogicalMode);

void DRAW_SetRectangle(Rectangle *r, uint16_t, uint16_t, uint16_t, uint16_t);
void DRAW_DrawRectangle(Rectangle *r);

//void DRAW_PutFontGlyph(VGA_Font *font, uint8_t code, uint16_t dest_x, uint16_t dest_y);
extern void DRAW_PutFontGlyph(__reg("a0") uint8_t *bitmap, __reg("d0") uint16_t dest_x, __reg("d1") uint16_t dest_y, __reg("d2") uint8_t code);

void DRAW_PutString(uint8_t *str, VGA_Font *font, uint16_t dest_x, uint16_t dest_y);

extern void DRAW_PutPixel(__reg("d0") uint16_t x, __reg("d1") uint16_t y);
#endif
