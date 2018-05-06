#include <string.h>
#include <stdio.h>

#include "devices/serial.h"
#include "fat.h"
#include "elf.h"
#include "memmgr.h"
#include "fastdraw.h"

void Monitor_DrawBanner();
void Monitor_InitPrompt();
void Monitor_WaitForEntry();
void Monitor_ClearPromptBuffer();
void Monitor_Go();
void Monitor_ProcessEntry();

void RunELF(char *path);
