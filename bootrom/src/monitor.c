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
extern void DRAW_ScreenFill(__reg("d0") uint8_t color);

void MonitorTask();
void VGATask();

void Monitor_Go()
{  
  DRAW_Init();
  VGACON_Init();
  TASK_InitSubsystem();

  MMIO8(0x600007) = MMIO8(0x600007) | 0x10; // enable MFP serial input interrupt
  MMIO8(0x600013) = MMIO8(0x600013) | 0x10; // enable MFP serial input interrupt
  DEVICE_InitSubsystem();
  DEVICE_Mouse_Create();
  DEVICE_Keyboard_Create();
  DEVICE_PrintAllDevices();

  printf("Testing message queue\n");
  DEVICE_Device *kbd = DEVICE_FindDeviceByName("keyboard.device");
  printf("Creating a message\n");
  KBD_KeyEvent *msg = IPC_CreateMessage(sizeof(KBD_KeyEvent), NULL);
  msg->keycode = 0x41;
  printf("Sending a message at %06X to port %06X\n", msg, &(kbd->message_port));
  IPC_SendMessage(msg, &(kbd->message_port));
  DEVICE_DoCommand("keyboard.device", CMD_OPEN);
  
  Task *task1 = MEMMGR_NewPtr(sizeof(Task)+8, H_SYSHEAP);
  TaskInfo *task1_info = MEMMGR_NewPtr(sizeof(TaskInfo), H_SYSHEAP);
  task1->info = task1_info;
  task1->info->heap = TASK_SYSHEAP;

  Task *task2 = MEMMGR_NewPtr(sizeof(Task)+8, H_SYSHEAP);
  TaskInfo *task2_info = MEMMGR_NewPtr(sizeof(TaskInfo), H_SYSHEAP);
  task2->info = MEMMGR_NewPtr(sizeof(TaskInfo), H_SYSHEAP);
  task2->info->heap = TASK_SYSHEAP;
  
  TASK_Add(task1, MonitorTask, NULL, 4096);
  TASK_Add(task2, VGATask, NULL, 4096);
  
  TASK_EnableSwitching(1);
  TASK_AllowInterrupts();
  
  printf("Waiting for scheduler...\n");  
  while(true) {};
  
  DEVICE_DoCommand("dev-mouse", 0);
  
  MOUSE_SetupCOM1();
  
  Monitor_DrawBanner();
  Monitor_InitPrompt();

  
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
  
  while(true)
	{
	  if(VGA_IsInVBLANK())
		DEMO_ProcessNextFrame();
	}
}

void MonitorTask()
{
  while(true)
	{
    /*
	  if(Monitor_WaitForEntry())
		{
		  Monitor_ProcessEntry();
		  Monitor_InitPrompt();
		}
    */
	}
}

void VGATask()
{
  DRAW_ScreenFill(0x07);
  
  // temporary
  Rectangle r;
  DRAW_SetPenFore(0x2F);
  DRAW_SetRectangle(&r, 50, 50, 40, 40);
  //DRAW_DrawRectangle(&r);

  VGACON_PutString("Video: Tseng Labs ET4000 - 1MB VRAM\n");
  VGACON_PutString("Procyon 68000 ROM 5/12/18\n");
  VGACON_PutString("\n");
  VGACON_PutString("DRAW_Polygon test screen\n");
  
  VGA_Bitmap PolluxVGA;
  PolluxVGA.data = BITMAP_PolluxVGA;
  PolluxVGA.size_x = 128;
  PolluxVGA.size_y = 128;
  PolluxVGA.vram_ptr = 0;

  DRAW_SetPenFore(0x3F);
  Point polygon[16];
  DRAW_MakePoint(&polygon[0], 200, 200);
  DRAW_MakePoint(&polygon[1], 250, 200);
  DRAW_MakePoint(&polygon[2], 275, 250);
  DRAW_MakePoint(&polygon[3], 200, 300);
  DRAW_MakePoint(&polygon[4], 140, 240);
  DRAW_PutPolygon(polygon, 5);

  DRAW_SetPenFore(0x2F);
  DRAW_MakePoint(&polygon[0], 400, 300);
  DRAW_MakePoint(&polygon[1], 375, 320);
  DRAW_MakePoint(&polygon[2], 330, 245);
  DRAW_PutPolygon(polygon, 3);

  while(true) {};
}

uint16_t shape_x = 50;
uint16_t shape_y = 50;
const uint32_t ZERO = 0;

void DEMO_ProcessNextFrame()
{
  /*
  //Let's do a DMA transfer!
  MMIO32(DMA_MAR(0)) = (uint32_t)&ZERO;
  MMIO32(DMA_DAR(0)) = 0x100000;
  MMIO16(DMA_MTCR(0)) = 0xFFFF; // longs
  MMIO8(DMA_DCR(0)) = DMA_XRM_BURST | DMA_DPS_16 | DMA_DTYP_ACK;
  MMIO8(DMA_OCR(0)) = DMA_SIZE_LONG | DMA_CHAIN_NONE | DMA_DIR_M2D;
  MMIO8(DMA_SCR(0)) = DMA_MAC_SAME | DMA_DAC_UP;
  MMIO8(DMA_CCR(0)) = DMA_STR_START;
  */

  //shape_x += 2;
  //shape_y += 2;

  Point polygon[4];
  DRAW_SetPenFore(0x4F);
  DRAW_MakePoint(&polygon[0], shape_x, shape_y);
  DRAW_MakePoint(&polygon[1], shape_x, shape_y+100);
  DRAW_MakePoint(&polygon[2], shape_x+100, shape_y+100);
  DRAW_MakePoint(&polygon[3], shape_x+100, shape_y);
  DRAW_PutPolygon(polygon, 4);

  if(shape_x >= 500)
	{
	  shape_x = 50;
	  shape_y = 50;
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

uint8_t Monitor_WaitForEntry()
{
  if(serial_char_waiting())
	{
	  char incoming = serial_in();
	  
	  if(incoming == 13) // CR
		{
		  return true;
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

  return false;
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
  HANDLE file_data = MEMMGR_NewHandle(file_size+1, H_SYSHEAP);
  
  FAT_ReadFile(drive_bpb[DRIVE_R],
			   fd,
			   *file_data,
			   file_size);

  ELF_LoadExecutable(*file_data);
}

void ISR_Spurious()
{
  //just for now
  printf("*** SPURIOUS INTERRUPT ***\n");
}

/* task testing */
int x = 0;
void testtask1()
{
  int y = 0;

  while(true)
	{
	  x++;
	  y--;
	  printf("testtask1: shared var is %d, non-shared is %d\n", x);
	};
}

void testtask2()
{
  int y = 0;

  while(true)
	{
	  x++;
	  y++;
	  printf("testtask2: shared var is %d, non-shared is %d\n", x);
	};
}
