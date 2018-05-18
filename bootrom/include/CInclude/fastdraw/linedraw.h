#ifndef LINEDRAW_H
#define LINEDRAW_H

#include <stdint.h>

#include "fastdraw/types.h"

extern Point penLocation;

void DRAW_PutPolygon(Point *points, uint16_t num);

#endif
