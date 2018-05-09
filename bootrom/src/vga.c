#include "vga.h"

VGA_Mode VGA_MODE;

/* TODO: move to vga.c */
void VGA_SetControllerRegister(uint16_t address_reg, uint16_t data_reg, uint8_t attrindex, uint8_t data)
{
  uint16_t tempaddr = MMIO16(VGA_IO_ADDRESS(address_reg));
  MMIO16(VGA_IO_ADDRESS(address_reg)) = attrindex;
  MMIO16(VGA_IO_ADDRESS(data_reg)) = data;
  MMIO16(VGA_IO_ADDRESS(address_reg)) = tempaddr;
}

void VGA_SetDMAColor(uint8_t r, uint8_t g, uint8_t b)
{
  MMIO16(VGA_IO_ADDRESS(0x3C9)) = r;
  MMIO16(VGA_IO_ADDRESS(0x3C9)) = g;
  MMIO16(VGA_IO_ADDRESS(0x3C9)) = b;
}

void VGA_SetBitmask(uint8_t mask)
{
  // Only bits that are enabled will be overwritten.
  MMIO16(VGA_IO_ADDRESS(0x3CE)) = 0x08;
  MMIO16(VGA_IO_ADDRESS(0x3CF)) = mask;
}

void VGA_SetBitplaneWriteMask(uint8_t bitplanes)
{
  //0x01 = plane 0
  //0x02 = plane 1
  //0x04 = plane 2
  //0x08 = plane 3

  MMIO16(VGA_IO_ADDRESS(0x3C4)) = 0x02;
  MMIO16(VGA_IO_ADDRESS(0x3C5)) = bitplanes;
}

void VGA_SetMode(enum VGA_Mode mode)
{
  VGA_MODE = mode;

  switch(mode)
	{
	case VGA_MODE_80x25:
	  VGA_Set80x25Mode();
	  break;
	case VGA_MODE_12h:
	  VGA_SetMode12h();
	  break;
	case VGA_MODE_13h:
	  VGA_SetMode13h();
	  break;
	case VGA_MODE_2Eh:
	  VGA_SetMode2Eh();
	}
}

void VGA_Set80x25Mode()
{
 // Set VGA to 80x25
  MMIO16(VGA_IO_ADDRESS(0x3C2)) = 0x67;
  
  int temp;
  temp = MMIO16(VGA_IO_ADDRESS(0x3D0)); // reset to index mode
  
  MMIO16(VGA_IO_ADDRESS(0x3C0)) = 0x10; // index
  MMIO16(VGA_IO_ADDRESS(0x3C0)) = 0x0C; // data

  MMIO16(VGA_IO_ADDRESS(0x3C0)) = 0x11;
  MMIO16(VGA_IO_ADDRESS(0x3C0)) = 0x00;
  
  MMIO16(VGA_IO_ADDRESS(0x3C0)) = 0x12;
  MMIO16(VGA_IO_ADDRESS(0x3C0)) = 0x0F;

  MMIO16(VGA_IO_ADDRESS(0x3C0)) = 0x13;
  MMIO16(VGA_IO_ADDRESS(0x3C0)) = 0x08;

  MMIO16(VGA_IO_ADDRESS(0x3C0)) = 0x14;
  MMIO16(VGA_IO_ADDRESS(0x3C0)) = 0x00;
  //
  MMIO16(VGA_IO_ADDRESS(0x3C4)) = 0x01;
  MMIO16(VGA_IO_ADDRESS(0x3C5)) = 0x00;

  MMIO16(VGA_IO_ADDRESS(0x3C4)) = 0x03;
  MMIO16(VGA_IO_ADDRESS(0x3C5)) = 0x00;

  MMIO16(VGA_IO_ADDRESS(0x3C4)) = 0x04;
  MMIO16(VGA_IO_ADDRESS(0x3C5)) = 0x07;

  MMIO16(VGA_IO_ADDRESS(0x3CE)) = 0x05;
  MMIO16(VGA_IO_ADDRESS(0x3CF)) = 0x10;

  MMIO16(VGA_IO_ADDRESS(0x3CE)) = 0x06;
  MMIO16(VGA_IO_ADDRESS(0x3CF)) = 0x0E;
  //
  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x00;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x5F;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x01;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x4F;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x02;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x50;
  
  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x03;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x82;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x04;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x55;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x05;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x81;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x06;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0xBF;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x07;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x1F;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x08;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x00;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x09;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x4F;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x10;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x9C;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x11;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x8E;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x12;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x8F;
  
  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x13;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x28;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x14;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x1F;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x15;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x96;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x16;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0xB9;

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x17;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0xA3;
}

void VGA_SetMode12h()
{
  // 640x480 16-color planar layout
  MMIO16(VGA_IO_ADDRESS(0x3C2)) = 0xE3;
  
  int temp;
  temp = MMIO16(VGA_IO_ADDRESS(0x3D0)); // reset to index mode

  //
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x10, 0x01);
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x11, 0x00);
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x12, 0x0F);
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x13, 0x00);
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x14, 0x00);
  
  //
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x01, 0x01);
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x03, 0x00);
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x04, 0x02);

  //
  VGA_SetControllerRegister(0x3CE, 0x3CF, 0x05, 0x00);
  VGA_SetControllerRegister(0x3CE, 0x3CF, 0x06, 0x05);
  VGA_SetControllerRegister(0x3CE, 0x3CF, 0x08, 0xFF);

  //
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x00, 0x5F);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x01, 0x4F);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x02, 0x50);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x03, 0x82);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x04, 0x54);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x05, 0x80);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x06, 0x0B);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x07, 0x3E);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x08, 0x00);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x09, 0x40);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x10, 0xEA);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x11, 0x8C);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x12, 0xDF);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x13, 0x28);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x14, 0x00);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x15, 0xE7);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x16, 0x04);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x17, 0xE3);  
}

void VGA_SetMode13h()
{
  // Set VGA to Mode 13h
  MMIO16(VGA_IO_ADDRESS(0x3C2)) = 0x63;
  
  int temp;
  temp = MMIO16(VGA_IO_ADDRESS(0x3D0)); // reset to index mode

  //
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x10, 0x41);
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x11, 0x00);
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x12, 0x0F);
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x13, 0x00);
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x14, 0x00);
  
  //
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x01, 0x01);
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x03, 0x00);
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x04, 0x0E);

  //
  VGA_SetControllerRegister(0x3CE, 0x3CF, 0x05, 0x40);
  VGA_SetControllerRegister(0x3CE, 0x3CF, 0x06, 0x05);
  VGA_SetControllerRegister(0x3CE, 0x3CF, 0x08, 0xFF);

  //
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x00, 0x5F);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x01, 0x4F);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x02, 0x50);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x03, 0x82);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x04, 0x54);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x05, 0x80);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x06, 0xBF);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x07, 0x1F);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x08, 0x00);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x09, 0x41);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x10, 0x9C);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x11, 0x8E);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x12, 0x8F);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x13, 0x28);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x14, 0x40);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x15, 0x96);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x16, 0xB9);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x17, 0xA3);

  // Writes go to all bitplanes.
  VGA_SetBitplaneWriteMask(0x0F);
}

void VGA_SetMode2Eh()
{
  // Set VGA to Mode 2Eh (640x480 linear)
  MMIO16(VGA_IO_ADDRESS(0x3C2)) = 0xE3;
  
  int temp;
  temp = MMIO16(VGA_IO_ADDRESS(0x3D0)); // reset to index mode

  //
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x10, 0x01);
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x11, 0x00);
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x12, 0x0F);
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x13, 0x00);
  VGA_SetControllerRegister(0x3C0, 0x3C0, 0x14, 0x00);
  
  //
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x00, 0x03);
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x01, 0x01);
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x02, 0x0F);
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x03, 0x00);
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x04, 0x0E);
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x06, 0x00);
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x07, 0xBC);

  //
  VGA_SetControllerRegister(0x3CE, 0x3CF, 0x05, 0x40);
  VGA_SetControllerRegister(0x3CE, 0x3CF, 0x06, 0x15);
  VGA_SetControllerRegister(0x3CE, 0x3CF, 0x07, 0x0F);
  VGA_SetControllerRegister(0x3CE, 0x3CF, 0x08, 0xFF);

  //
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x00, 0x5F);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x01, 0x4F);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x02, 0x50);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x03, 0x82);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x04, 0x54);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x05, 0x80);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x06, 0x0B);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x07, 0x3E);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x08, 0x00);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x09, 0x40);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x10, 0xEA);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x11, 0x8C);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x12, 0xDF);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x13, 0x50);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x14, 0x60);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x15, 0xE7);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x16, 0x04);
  VGA_SetControllerRegister(0x3D4, 0x3D5, 0x17, 0xAB);
  VGA_SetControllerRegister(0x3C4, 0x3C5, 0x18, 0xFF);

  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x24;
  MMIO16(VGA_IO_ADDRESS(0x3D5)) = 0x10;
  MMIO16(VGA_IO_ADDRESS(0x3D4)) = 0x00;
  
  VGA_SetBitplaneWriteMask(0x0F);
}

void VGA_SetWriteMode(int mode)
{
  MMIO16(VGA_IO_ADDRESS(0x3CE)) = 0x05;
  uint16_t current = MMIO16(VGA_IO_ADDRESS(0x3CF)) & 0x00FC;
  MMIO16(VGA_IO_ADDRESS(0x3CF)) = current | mode;
}

void VGA_SetMode13Pixel(uint16_t x, uint16_t y, uint8_t color)
{
  //Calculate the pixel offset.
  uint32_t offset = (y*320) + x;
  MMIO16(VGA_MEM_ADDRESS(0xA0000 + offset)) = color;
}

void VGA_SetStandardPalette()
{
  MMIO16(VGA_IO_ADDRESS(0x3C8)) = 0x00;

  //Each call to VGA_SetDMAColor advances the DMA pointer.

  // Standard VGA text colors.
  VGA_SetDMAColor(0, 0, 0);
  VGA_SetDMAColor(0, 0, 42);
  VGA_SetDMAColor(0, 42, 0);
  VGA_SetDMAColor(0, 42, 42);
  VGA_SetDMAColor(42, 0, 0);
  VGA_SetDMAColor(42, 0, 42);
  VGA_SetDMAColor(42, 21, 0);
  VGA_SetDMAColor(0, 42, 42);

  VGA_SetDMAColor(21, 21, 21);
  VGA_SetDMAColor(21, 21, 63);
  VGA_SetDMAColor(21, 63, 21);
  VGA_SetDMAColor(21, 63, 63);
  VGA_SetDMAColor(63, 21, 21);
  VGA_SetDMAColor(63, 21, 63);
  VGA_SetDMAColor(63, 63, 21);
  VGA_SetDMAColor(63, 63, 63);

  for(int i=0;i<240; i++)
	{
	  VGA_SetDMAColor(63, 63, 63);
	}
  
  // Set 16 palette entries.
  for(int i=0; i<256; i++)
	{
	  int discard = MMIO16(VGA_IO_ADDRESS(0x3DA));
	  MMIO16(VGA_IO_ADDRESS(0x3C0)) = i;
	  MMIO16(VGA_IO_ADDRESS(0x3C0)) = i;
	}

  // Re-enable the palette.
  MMIO16(VGA_IO_ADDRESS(0x3C0)) = 0x20;
}

/* 
   This bit controls the map selected during system read operations.
   When set to 0, this bit enables system addresses to sequentially access data within a bit map by using the Map Mask register.
   When set to 1, this bit causes the two low-order bits to select the map accessed. 
*/
void VGA_ChainPlanes()
{
  MMIO16(VGA_IO_ADDRESS(0x3C4)) = 0x04;
  MMIO16(VGA_IO_ADDRESS(0x3C5)) = MMIO16(VGA_IO_ADDRESS(0x3C5)) | 0x04;
}

void VGA_UnchainPlanes()
{
  MMIO16(VGA_IO_ADDRESS(0x3C4)) = 0x04;
  MMIO16(VGA_IO_ADDRESS(0x3C5)) = MMIO16(VGA_IO_ADDRESS(0x3C5)) & ~0x04;
}

void VGA_PutBitmap(VGA_Bitmap *bitmap, uint16_t dest_x, uint16_t dest_y, uint16_t source_x, uint16_t source_y, uint16_t size_x, uint16_t size_y, BITMAP_FLAGS flags)
{
  uint32_t ptr = 0;
  uint16_t add_after_scanline = bitmap->size_x - size_x;

  uint16_t offset = (bitmap->size_x * source_y) + source_x;
  uint8_t *pixels = bitmap->data + offset;

  if(flags & BITMAP_FLIP_Y)
	{
	  for(int row = dest_y+size_y; row > dest_y; row--)
		{
		  for(int column=dest_x; column < dest_x+size_x; column++)
			{
			  VGA_SetPixel(column, row, pixels[ptr++]);
			}
		  ptr += add_after_scanline;
		}
	}
  else
	{
	  for(int row = dest_y; row < dest_y+size_y; row++)
		{
		  for(int column=dest_x; column < dest_x+size_x; column++)
			{
			  VGA_SetPixel(column, row, pixels[ptr++]);
			}
		  ptr += add_after_scanline;
		}
	}
}
