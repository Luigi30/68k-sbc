#include "fastdraw/linedraw.h"

/*
void DRAW_LineTo(uint16_t x1, uint16_t y1)
{  
  int16_t deltaX = x1 - penLocation.x;
  int16_t deltaY = y1 - penLocation.y;

  if(deltaX < 0)
	deltaX = deltaX * -1;
  if(deltaY > 0)
	deltaY = deltaY * -1;

  int16_t addX = penLocation.x < x1 ? 1 : -1;
  int16_t addY = penLocation.y < y1 ? 1 : -1;
  
  int16_t error = deltaX + deltaY;
  int16_t error2;

  while(1)
	{
	  DRAW_PutPixel(penLocation.x, penLocation.y);
	  if(penLocation.x == x1 && penLocation.y == y1) break;

	  error2 = 2 * error;
	  if(error2 >= deltaY)
		{
		  error += deltaY;
		  penLocation.x += addX; 
		}
	  if(error2 <= deltaX)
		{
		  error += deltaX;
		  penLocation.y += addY;
		}
	}
}
*/
