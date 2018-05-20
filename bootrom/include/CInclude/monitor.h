#include <string.h>
#include <stdio.h>

#include "devices/serial.h"
#include "fat.h"
#include "elf.h"
#include "memmgr.h"
#include "fastdraw.h"
#include "fastdraw/linedraw.h"
#include "vgaconsole.h"
#include "mouse.h"

void Monitor_DrawBanner();
void Monitor_InitPrompt();

uint8_t Monitor_WaitForEntry();
void Monitor_ClearPromptBuffer();
void Monitor_Go();
void Monitor_ProcessEntry();

void RunELF(char *path);

void DEMO_ProcessNextFrame();
