#include "vgaconsole.h"

typedef struct {
  uint8_t text[VGACONSOLE_COLS];
  uint8_t dirty;
} VGACON_Row;

VGACON_Row VGACON_TextRows[VGACONSOLE_ROWS];

void VGACON_Init()
{
  console_font = (VGA_Font *)malloc(sizeof(VGA_Font));
  console_font->bitmap.data = BITMAP_Fixedsys12;
  console_font->bitmap.size_x = 256;
  console_font->bitmap.size_y = 256;
  console_font->bitmap.vram_ptr = 0;
  console_font->glyph_x = 10;
  console_font->glyph_y = 16;
  
  for(int i=0;i<VGACONSOLE_ROWS;i++)
	{
	  for(int j=0;j<VGACONSOLE_COLS;j++)
		VGACON_TextRows[i].text[j] = '\0';
	}

  cursor_x = 0;
  cursor_y = 0;
}

void VGACON_UpdatePort()
{
  for(int i=0; i<VGACONSOLE_ROWS; i++)
	{
	  if(VGACON_TextRows[i].dirty)
		{
		  VGACON_TextRows[i].dirty = 0;
		  DRAW_PutString(VGACON_TextRows[i].text, console_font, 0, i * 16);
		}
	}
}

void VGACON_SetFont(VGA_Font *font)
{
  console_font = font;
}

void VGACON_PutString(char *str)
{
  VGACON_TextRows[cursor_y].dirty = 1;
  
  for(int i=0; i<strlen(str); i++)
	{
	  if(str[i] < 0x20)
		VGACON_HandleNonPrintable(str[i]);
	  else
		VGACON_TextRows[cursor_y].text[cursor_x++] = str[i];
	}
  VGACON_UpdatePort();
}

void VGACON_HandleNonPrintable(char c)
{
  if(c == '\n')
	{
	  cursor_x = 0;
	  cursor_y++;
	}

  while(cursor_y >= VGACONSOLE_ROWS)
	{
	  VGACON_ScrollRowsUp(1);
	  cursor_y--;
	}
}

void VGACON_ScrollRowsUp(uint8_t num)
{
  for(int count=0; count < num; count++)
	{
	  if(cursor_y >= VGACONSOLE_ROWS)
		{
		  for(int row=1; row<VGACONSOLE_ROWS; row++)
			{
			  VGACON_TextRows[row-1].dirty = 1;
			  for(int col=0; col<VGACONSOLE_COLS; col++)
				VGACON_TextRows[row-1].text[col] = VGACON_TextRows[row].text[col];
			}
		  cursor_y--;
		}
	}
}
