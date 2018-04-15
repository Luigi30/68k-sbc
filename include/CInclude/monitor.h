#include <string.h>
#include <stdio.h>

#include "serial.h"
#include "memmgr.h"

void Monitor_DrawBanner();
void Monitor_InitPrompt();
void Monitor_WaitForEntry();
void Monitor_ClearPromptBuffer();
void Monitor_Go();
void Monitor_ProcessEntry();
