#ifndef LINEDRAW_H
#define LINEDRAW_H

#include <stdint.h>

/* Draw a line with the fore pen color from the pen position to x,y.
   Move pen when complete. */
void DRAW_LineTo(uint16_t x, uint16_t y);

#endif
