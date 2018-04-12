#include "monitor.h"

#define INPUT_BUFFER_SIZE 256
int input_buffer_position = 0;

char inputBuffer[INPUT_BUFFER_SIZE];
char STR_BootBanner[] = "Procyon 68000 ROM Monitor - 2018-04-10";
char STR_CRLF[] = "\r\n";
char STR_CommandPrompt[] = "> ";

void Monitor_Go()
{  
  Monitor_DrawBanner();
  Monitor_InitPrompt();

  printf("Memory Manager test\n");
  MEMMGR_HANDLE hndl = MEMMGR_AllocMem(128, MEM_NOFLAGS);
  printf("hndl: $%06X\n", hndl);
  
  while(TRUE)
	{
	  Monitor_WaitForEntry();
	  Monitor_ProcessEntry();
	  Monitor_InitPrompt();
	}
}

void Monitor_DrawBanner()
{
  serial_string_out(STR_BootBanner);
}

void Monitor_InitPrompt()
{
  serial_string_out(STR_CRLF);
  serial_string_out(STR_CRLF);
  Monitor_ClearPromptBuffer();
}

void Monitor_WaitForEntry()
{
  int waiting = TRUE;
  serial_string_out(STR_CommandPrompt);
  
  while(waiting) {
	if(serial_char_waiting())
	  {
		char incoming = serial_in();
		inputBuffer[input_buffer_position++] = incoming;
		serial_char_out(incoming);

		if(incoming == 13)
		  {
			waiting = FALSE;
		  }
	  }
  }
}

void Monitor_ClearPromptBuffer()
{
  input_buffer_position = 0;
  
  for(int i=0;i<INPUT_BUFFER_SIZE;i++)
	{
	  inputBuffer[i] = '\0';
	}  
}

#define CMD_EXAMINE 'e'
#define CMD_RUN 'r'
#define CMD_SETMEM 's'
#define CMD_DUMPMEM 'd'
#define CMD_ROMBOOT 'b'

void Monitor_ProcessEntry()
{
  serial_string_out(STR_CRLF);
  serial_string_out(STR_CRLF);

  /* What command is this? */

  /* TODO: If a value follows the command,
	 try parsing it as a hex number. */
  
  if(inputBuffer[0] == CMD_EXAMINE)
	{
	  serial_string_out("EXAMINE...\n");
	}
  else if(inputBuffer[0] == CMD_RUN)
	{
	  serial_string_out("RUN...\n");
	}
  else if(inputBuffer[0] == CMD_SETMEM)
	{
	  serial_string_out("SET MEMORY...\n");
	}
  else if(inputBuffer[0] == CMD_DUMPMEM)
	{
	  serial_string_out("DUMP MEMORY...\n");
	}
  else if(inputBuffer[0] == CMD_ROMBOOT)
	{
	  serial_string_out("ROM BOOT\n");
	  FAT_BootROM();
	}
  else
	{
	  serial_string_out("Invalid command");
	}
}
