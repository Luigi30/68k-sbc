#include "monitor.h"

DMA_ChainBlock DMA_Chain[500];

#define INPUT_BUFFER_SIZE 256
int input_buffer_position = 0;

char inputBuffer[INPUT_BUFFER_SIZE];
const char STR_BootBanner[] = "Procyon 68000 ROM Monitor - 2018-04-17";
const char STR_CRLF[] = "\r\n";
const char STR_CommandPrompt[] = "> ";

DrawPort *currentPort;

extern void DRAW_LineTo(__reg("d0") uint16_t x, __reg("d1") uint16_t y);

void Monitor_Go()
{  
  DRAW_Init();
  VGACON_Init();

  Monitor_DrawBanner();
  Monitor_InitPrompt();
  
  // temporary
  
  Rectangle r;
  DRAW_SetPenFore(0xFF);
  DRAW_SetRectangle(&r, 50, 50, 40, 40);
  //DRAW_DrawRectangle(&r);

  VGACON_PutString("Video: Tseng Labs ET4000 - 1MB VRAM\n");
  VGACON_PutString("Procyon 68000 ROM 5/12/18\n");
  VGACON_PutString("\n");
  VGACON_PutString("DRAW_MovePen and DRAW_LineTo octant test screen\n");
  for(int i=0;i<16;i++)
	{
	  //VGACON_PutString("Hello World! This is a row ended with a newline character.\n");
	}
  
  VGA_Bitmap PolluxVGA;
  PolluxVGA.data = BITMAP_PolluxVGA;
  PolluxVGA.size_x = 128;
  PolluxVGA.size_y = 128;
  PolluxVGA.vram_ptr = 0;

  DRAW_MovePen(0, 0);
  DRAW_LineTo(320, 240);
  
  DRAW_MovePen(160, 0);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(320, 0);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(480, 0);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(639, 0);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(0, 479);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(160, 479);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(320, 479);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(480, 479);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(639, 479);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(0, 120);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(0, 240);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(0, 360);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(639, 120);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(639, 240);
  DRAW_LineTo(320, 240);

  DRAW_MovePen(639, 360);
  DRAW_LineTo(320, 240);
  
  /*
  //Let's do a DMA transfer!
  //MMIO32(DMA_MAR(0)) = (uint32_t)fixedsys12.bitmap.data;
  MMIO32(DMA_DAR(0)) = 0x100000;
  //MMIO16(DMA_MTCR(0)) = 128*256; // in words
  MMIO8(DMA_DCR(0)) = DMA_XRM_BURST | DMA_DPS_16 | DMA_DTYP_ACK;
  //MMIO8(DMA_OCR(0)) = DMA_SIZE_WORD | DMA_CHAIN_NONE | DMA_DIR_M2D;
  MMIO8(DMA_OCR(0)) = DMA_SIZE_WORD | DMA_CHAIN_ARRAY | DMA_DIR_M2D;
  MMIO8(DMA_SCR(0)) = DMA_MAC_UP | DMA_DAC_UP;
  MMIO32(DMA_BAR(0)) = (uint32_t)DMA_Chain;
  MMIO16(DMA_BTCR(0)) = 256;

  for(int i=0;i<256;i++)
	{
	  DMA_Chain[i].source = (uint8_t *)fixedsys12.bitmap.data + (i << 8);
	  DMA_Chain[i].transfer_count = 128;
	}

  MMIO8(DMA_CCR(0)) = DMA_STR_START;
  */
  
  //VGA_PutBitmap(&PolluxVGA, 0, 256, 0, 0, 128, 128, BITMAP_FLIP_Y);
  //DRAW_PutFontGlyph(&fixedsys12, 'A', 384, 384);
  //DRAW_PutString("Hello! I present really slow text printing.", &fixedsys12, 0, 0);
  //DRAW_PutString("With multiple fonts!", &F_8bitpusab12, 0, 16);

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

		if(incoming == 13) // CR
		  {
			waiting = FALSE;
		  }
		else if(incoming == 8)  // BS
		  {
			inputBuffer[input_buffer_position--] = 0x00;
		  }
		else {
		  inputBuffer[input_buffer_position++] = incoming;
		}

		serial_char_out(incoming);
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
#define CMD_RUNELF 'x'

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
	  MEMMGR_DumpHeapBlocks(&heap_system);
	  FAT_BootROM();
	}
  else if(inputBuffer[0] == CMD_RUNELF)
	{
	  serial_string_out("RUN ELF\n");
	  RunELF("R:\\TEST.ELF");
	}
  else
	{
	  serial_string_out("Invalid command");
	}
}

void RunELF(char *path)
{
  FAT_MountDrive(DRIVE_R);

  int fd = FAT_OpenFile(path, FILE_FLAG_READ);
  uint32_t file_size = FAT_GetFileSize(fd);
  HANDLE file_data = MEMMGR_NewHandle(file_size+1);
  
  FAT_ReadFile(drive_bpb[DRIVE_R],
			   fd,
			   *file_data,
			   file_size);

  ELF_LoadExecutable(*file_data);
}
