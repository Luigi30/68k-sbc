#include "monitor.h"

DMA_ChainBlock DMA_Chain[500];

#define INPUT_BUFFER_SIZE 256
int input_buffer_position = 0;

char inputBuffer[INPUT_BUFFER_SIZE];
const char STR_BootBanner[] = "Procyon 68000 ROM Monitor - 2018-04-17";
const char STR_CRLF[] = "\r\n";
const char STR_CommandPrompt[] = "> ";

DrawPort *currentPort;

void Monitor_Go()
{  
  Monitor_DrawBanner();
  Monitor_InitPrompt();

  // temporary
  screenPort.vram_base = (uint8_t *)0x900000;
  currentPort = &screenPort;
  DRAW_SetVGAMode(VGA_MODE_2Eh);
  VGA_SetStandardPalette();
  //VGA_SetWriteMode(2);
  //VGA_SetBitplaneWriteMask(0x0F);
  //VGA_ChainPlanes();
  
  Rectangle r;
  DRAW_SetPenFore(0x0F);
  DRAW_SetRectangle(&r, 50, 50, 40, 40);
  DRAW_DrawRectangle(&r);
  DRAW_PutPixel(256, 256);

  VGA_Font fixedsys12;
  fixedsys12.bitmap.data = BITMAP_Fixedsys12;
  fixedsys12.bitmap.size_x = 256;
  fixedsys12.bitmap.size_y = 256;
  fixedsys12.bitmap.vram_ptr = 0;
  fixedsys12.glyph_x = 10;
  fixedsys12.glyph_y = 16;

  DRAW_PutFontGlyph(fixedsys12.bitmap.data, 256, 256, 'A');
  for(int i=0;i<16;i++)
	{
	  DRAW_PutString("Hello World!", &fixedsys12, 0, i<<4);
	}
  
  VGA_Font F_8bitpusab12;
  F_8bitpusab12.bitmap.data = BITMAP_8bitpusab12;
  F_8bitpusab12.bitmap.size_x = 256;
  F_8bitpusab12.bitmap.size_y = 256;
  F_8bitpusab12.bitmap.vram_ptr = 0;
  F_8bitpusab12.glyph_x = 10;
  F_8bitpusab12.glyph_y = 16;

  VGA_Bitmap PolluxVGA;
  PolluxVGA.data = BITMAP_PolluxVGA;
  PolluxVGA.size_x = 128;
  PolluxVGA.size_y = 128;
  PolluxVGA.vram_ptr = 0;
  
  //VGA_SetPixel(0, 0, 0x0F);
  for(int i=0;i<10;i++){
	VGA_SetPixel(40+i, 40, 0x01 | 0x02 | 0x04 | 0x08);
  }

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
  //VGA_PutBitmap(&fixedsys12, 400, 300, 0, 0, 256, 256);
  //VGA_PutBitmap(&fixedsys12, 0, 0, 0, 0, 256, 256, BITMAP_NONE);
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
