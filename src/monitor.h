#include <string.h>
#include "serial.h"

#define TRUE 1
#define FALSE 0

void Monitor_DrawBanner();
void Monitor_InitPrompt();
void Monitor_WaitForEntry();
void Monitor_ClearPromptBuffer();
void Monitor_Go();
void Monitor_ProcessEntry();
