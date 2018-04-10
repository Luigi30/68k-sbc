#include "monitor.h"

#define INPUT_BUFFER_SIZE 256
int input_buffer_position = 0;

char inputBuffer[INPUT_BUFFER_SIZE];
char STR_BootBanner[] = "Luigi's 68K SBC ROM Monitor - 2018-04-09";
char STR_CRLF[] = "\r\n";
char STR_CommandPrompt[] = "> ";

void Monitor_Go()
{
  Monitor_DrawBanner();
  Monitor_InitPrompt();

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
	  serial_string_out("EXAMINE...");
	}
  else if(inputBuffer[0] == CMD_RUN)
	{
	  serial_string_out("RUN...");
	}
  else if(inputBuffer[0] == CMD_SETMEM)
	{
	  serial_string_out("SET MEMORY...");
	}
  else if(inputBuffer[0] == CMD_DUMPMEM)
	{
	  serial_string_out("DUMP MEMORY...");
	}
  else if(inputBuffer[0] == CMD_ROMBOOT)
	{
	  serial_string_out("ROM BOOT");
	  FAT_BootROM();
	}
  else
	{
	  serial_string_out("Invalid command");
	}
}
