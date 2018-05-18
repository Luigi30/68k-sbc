#include "fastdraw/linedraw.h"

extern void DRAW_LineTo(__reg("d0") uint16_t x, __reg("d1") uint16_t y);
void DRAW_PutPolygon(Point *points, uint16_t num)
{
  DRAW_MovePenToPoint(points[0]);
  
  for(int i=1; i<num; i++)
	{
	  DRAW_LineTo(points[i].x, points[i].y);
	}

  DRAW_LineTo(points[0].x, points[0].y);
}
