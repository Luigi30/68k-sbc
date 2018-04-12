#include "fat.h"

#define ROMFS_START 0x80000

uint16_t swap_word_endianness(uint16_t value)
{
  return (uint16_t)(((value & 0xFF) << 16) | ((value & 0xFF00) >> 16));
}

uint16_t get_swapped_word(uint8_t *location)
{
  return (uint16_t)((MMIO8(location+1) << 8) | MMIO8(location));
}

uint32_t get_swapped_long(uint8_t *location)
{
  return (uint32_t)((MMIO8(location+3) << 24) |
					(MMIO8(location+2) << 16) |
					(MMIO8(location+1) << 8) |
					(MMIO8(location)));
}

void FAT_BootROM()
{
  malloc(128);
  malloc(128);
  
  /* Identify the disk at ROMFS_START. */
  char buffer[64];
  int bootstrap = (MMIO32(ROMFS_START) >> 8);
  itoa(bootstrap, buffer, 16);

  printf("\n");

  FAT_ReadBPB(&rom_bpb, (char *)ROMFS_START);
  FAT_PrintBPBInfo(&rom_bpb);

  char sector_decoded_buffer[rom_bpb.bytes_per_sector];

  /* Read a 10-sector FAT12 FAT. */
  FAT_DecodeFAT12FAT(rom_bpb,
					 (char *)ROMFS_START+(rom_bpb.bytes_per_sector),
					 rom_fat);
  printf("FAT: Read FAT12 FAT into memory at %06X\n", &rom_fat);

  FAT_DumpRootDirectory(rom_bpb,
						rom_root_dir,
						SECTOR_OFFSET(ROMFS_START,
									  rom_bpb.bytes_per_sector,
									  1+(rom_bpb.fat_copies*FAT12_FAT_SECTOR_COUNT)));
  
}

void FAT_ReadBPB(FAT_BPB *bpb, char *data)
{
  bpb->bytes_per_sector = get_swapped_word(data+11);
  bpb->sectors_per_cluster = MMIO8(data+13);
  bpb->reserved_sectors = get_swapped_word(data+14);
  bpb->fat_copies = MMIO8(data+16);
  bpb->root_directory_entries = get_swapped_word(data+17);
  bpb->total_sectors = get_swapped_word(data+19);
  bpb->media_descriptor_type = MMIO8(data+21);
  bpb->sectors_per_fat = get_swapped_word(data+22);
  bpb->sectors_per_track = get_swapped_word(data+24);
  bpb->heads = get_swapped_word(data+26);
  bpb->hidden_sectors = get_swapped_word(data+28);
  if(MMIO8(data+38) == 0x29)
	{ 
	  bpb->extended_fields_present = 1;
	  strncpy(bpb->volume_label, data+43, 11);
	  strncpy(bpb->filesystem_type, data+54, 8);
	}
  else
	bpb->extended_fields_present = 0;
}

void FAT_PrintBPBInfo(FAT_BPB *bpb)
{
  char buffer[64];
  printf("*** BPB DATA ***\n");
  sprintf(buffer, "Dumping BPB at $%06x\n", bpb);
  printf(buffer);
  
  sprintf(buffer, "Media descriptor type is $%02X\n", bpb->media_descriptor_type);
  printf(buffer);

  sprintf(buffer, "Volume has %d sectors per FAT\n", bpb->sectors_per_fat);
  printf(buffer);

  sprintf(buffer, "Volume has %d sectors per cluster\n", bpb->sectors_per_cluster);
  printf(buffer);

  sprintf(buffer, "Volume has %d bytes per sector\n", bpb->bytes_per_sector);
  printf(buffer);

  sprintf(buffer, "Volume has %d sectors - calculated size is %d bytes\n",
		  bpb->total_sectors, bpb->bytes_per_sector * bpb->total_sectors);
  printf(buffer);

  sprintf(buffer, "Volume has %d reserved sectors\n", bpb->reserved_sectors);
  printf(buffer);

  sprintf(buffer, "Volume has %d root directory entries\n", bpb->root_directory_entries);
  printf(buffer);

  if(bpb->extended_fields_present)
	{
	  printf("Extended BPB fields present\n");
	  printf("Volume label is %s\n", bpb->volume_label);
	  printf("Reported filesystem type is %s\n", bpb->filesystem_type);
	}  
  else
	printf("No extended BPB field signature\n");
}

void FAT_DecodeFAT12FAT(FAT_BPB bpb, char *sector, uint16_t *decoded_fat)
{
  /* Decodes one sector from a disk.
	 Expects a pointer to a sector. */

  /* TODO: more than 512 bytes per sector */

  //  for(int i=0; i<256*FAT12_FAT_SECTOR_COUNT; i++)
  for(int i=0; i<(512/3)*FAT12_FAT_SECTOR_COUNT; i++)
	{
	  uint32_t encoded_24 = 0;
	  uint16_t decoded_high = 0;
	  uint16_t decoded_low = 0;

	  encoded_24 = (MMIO8(sector + i*3) << 24) |
		(MMIO8(sector + i*3 + 1) << 16) |
		(MMIO8(sector + i*3 + 2) << 8);
	  
	  decoded_high = (encoded_24 & 0xFF000000) >> 24 | (encoded_24 & 0x000F0000) >> 8;
	  decoded_low = (encoded_24 & 0x00F00000) >> 20 | (encoded_24 & 0x0000FF00) >> 4;

	  decoded_fat[i*2] = decoded_high;
	  decoded_fat[i*2+1] = decoded_low;

	  //printf("%8x (%d) - decoded_high: %x, decoded_low: %x\n", encoded_24, i%2, decoded_high, decoded_low);
	}
}

void FAT_DumpRootDirectory(FAT_BPB bpb, FAT_ROOT_DIRECTORY_ENTRY *dir_list, char *first_sector)
{
  /* Endianness... */
  printf("*** ROOT DIRECTORY ***\n");
  char *entry_ptr = first_sector;
  
  for(int i=0; i<bpb.root_directory_entries; i++)
	{
	  char is_directory = 0;
	  char flag = entry_ptr[0];
	  if(flag == 0x00 || flag == 0xE5)
		continue;

	  uint32_t file_size = get_swapped_long(entry_ptr+28);
	  dir_list[i].file_size = file_size;
	  
	  dir_list[i].allocation_or_first_letter = flag;
	  strncpy(dir_list[i].filename, entry_ptr+1, 11);
	  printf("%c%s\t", dir_list[i].allocation_or_first_letter, dir_list[i].filename);

	  if(is_directory)
		{
		  printf("<DIR>");
		}
	  else {
		printf("%d", dir_list[i].file_size);
	  }
	  printf("\n");

	  entry_ptr += 32;
	}  
}
