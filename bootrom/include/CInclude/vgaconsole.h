#ifndef VGACONSOLE_H
#define VGACONSOLE_H

#include <stdint.h>
#include <string.h>

#include "fastdraw.h"

#define VGACONSOLE_ROWS 25
#define VGACONSOLE_COLS 64

static VGA_Font *console_font;
static uint8_t cursor_x, cursor_y;

void VGACON_Init();
void VGACON_UpdatePort();
void VGACON_SetFont(VGA_Font *font);
void VGACON_PutString(char *str);
void VGACON_HandleNonPrintable(char c);
void VGACON_ScrollRowsUp(uint8_t num);
#endif
