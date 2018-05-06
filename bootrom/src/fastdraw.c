#include "fastdraw.h"

void DRAW_SetPenFore(uint8_t color_index) { pen_fore = color_index; }
void DRAW_SetPenBack(uint8_t color_index) { pen_back = color_index; }

/* Rectangles! */
void DRAW_SetRectangle(Rectangle *r, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  r->x = x;
  r->y = y;
  r->width = width;
  r->height = height;
}

void DRAW_DrawRectangle(Rectangle *r)
{
  for(int i=r->x; i < r->x + r->width; i++)
	{
	  VGA_SetPixel(i, r->y, pen_fore);
	  VGA_SetPixel(i, r->y + r->height - 1, pen_fore);
	}

  for(int i=r->y; i< r->y + r->height; i++)
	{
	  VGA_SetPixel(r->x, i, pen_fore);
	  VGA_SetPixel(r->x + r->width - 1, i, pen_fore);
	}
}

void DRAW_SetLogicalMode(DRAW_LogicalMode mode)
{
  MMIO16(VGA_IO_ADDRESS(0x3CE)) = 0x03;
  uint8_t current = (uint8_t)MMIO16(VGA_IO_ADDRESS(0x3CF));
  current = current & 0xE7;
  MMIO16(VGA_IO_ADDRESS(0x3CF)) = current | mode;
}
