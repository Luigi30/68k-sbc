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
  // Clear all drive letters.
  for(int i=0; i<26; i++)
	drive_bpb[i] = NULL;

  for(int i=0;i<FILES_LIMIT;i++)
	{
	  file_descriptor_table[i].id = i;
	  file_descriptor_table[i].flags = FILE_FLAG_UNUSED;
	  memset(file_descriptor_table[i].path, 0x00, 128);
	}

  for(int i=0;i<224;i++)
	{
	  rom_root_dir[i].allocation_or_first_letter = 0x00;
	  rom_root_dir[i].filename[0] = 0x00;
	}
  
  /* Identify the disk at ROMFS_START. */
  char buffer[64];
  int bootstrap = (MMIO32(ROMFS_START) >> 8);
  itoa(bootstrap, buffer, 16);

  printf("\n");

  FAT_ReadBPB(&rom_bpb, DRIVE_R, (char *)ROMFS_START);
  FAT_PrintBPBInfo(DRIVE_R);
  drive_bpb[DRIVE_R] = &rom_bpb;
  drive_fat[DRIVE_R] = rom_fat;
  drive_root_dir[DRIVE_R] = rom_root_dir;
  
  char sector_decoded_buffer[rom_bpb.bytes_per_sector];
    
  /* Read a 10-sector FAT12 FAT. */
  FAT_DecodeFAT12FAT((char *)(ROMFS_START+FAT_OffsetFATStart(drive_bpb[DRIVE_R])),
					 rom_fat);
  printf("FAT: Read FAT12 FAT into memory at %06X\n", &rom_fat);

  FAT_DumpRootDirectory(DRIVE_R,
						rom_root_dir,
						(char *)(0x80000 + FAT_OffsetRootDirStart(drive_bpb[DRIVE_R])));

  printf("ROM drive is open as R:\n");
  
  // Try to open and read ASCII.TXT.
  char file_to_open[] = "R:\\ASCII.TXT";
  printf("Trying to open file %s.\n", file_to_open);
  uint16_t fd = FAT_OpenFile("R:\\ASCII.TXT", FILE_FLAG_READ);
  printf("Got file descriptor %d\n", fd);

  uint32_t file_size = file_descriptor_table[fd].root_dir_entry->file_size;
  HANDLE file_data = MEMMGR_NewHandle(file_size+1);
  FAT_ReadFile(drive_bpb[DRIVE_R], fd, *file_data, file_size);

  printf("\n*** Printing File ***\n");
  printf(*file_data);
  printf("\n*** Print Complete ***\n");
}

//void FAT_ReadBPB(FAT_BPB *bpb, char *data)
void FAT_ReadBPB(FAT_BPB *bpb, DRIVE_LETTER drive, char *data)
{
  drive_bpb[drive] = bpb;
  
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

void FAT_PrintBPBInfo(DRIVE_LETTER drive)
{
  FAT_BPB *bpb = drive_bpb[drive];
  
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

void FAT_DecodeFAT12FAT(char *sector, uint16_t *decoded_fat)
{
  /* Decodes one sector from a disk.
	 Expects a pointer to a sector. */

  /* TODO: more than 512 bytes per sector */

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
	}
}

void FAT_DumpRootDirectory(DRIVE_LETTER drive, FAT_ROOT_DIRECTORY_ENTRY *dir_list, char *first_sector)
{
  FAT_BPB *bpb = drive_bpb[drive];
  
  /* Endianness... */
  printf("*** ROOT DIRECTORY ***\n");
  char *entry_ptr = first_sector;
  
  for(int i=0; i<bpb->root_directory_entries; i++)
	{
	  char is_directory = 0;
	  char flag = entry_ptr[0];
	  if(flag == 0x00 || flag == 0xE5)
		continue;

	  printf("ID %03d ", i);
	  
	  uint32_t file_size = get_swapped_long(entry_ptr+28);
	  dir_list[i].file_size = file_size;
	  
	  dir_list[i].allocation_or_first_letter = flag;
	  strncpy(dir_list[i].filename, entry_ptr+1, 7);
	  dir_list[i].filename[7] = '.';
	  strncpy(dir_list[i].filename+8, entry_ptr+8, 3);
	  printf("%c%s\t", dir_list[i].allocation_or_first_letter, dir_list[i].filename);

	  dir_list[i].first_cluster_low = get_swapped_word(entry_ptr+0x1A);
	  
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

uint16_t FAT_OpenFile(char *path, FAT_FILE_FLAGS mode)
{
  FAT_FILE_DESCRIPTOR *fd = FAT_GetUnusedFileDescriptor();

  if(mode > 0x00 && mode < 0x04)
	fd->flags = mode;
  else
	return FAT_ERROR_INVALID_FILE_MODE;

  fd->root_dir_entry = FAT_SearchRootDirectory(rom_root_dir, path+3);

  strcpy(fd->path, path);
  
  return fd->id;
}

FAT_FILE_DESCRIPTOR *FAT_GetUnusedFileDescriptor()
{
  for(int i=0;i<FILES_LIMIT;i++)
	{
	  if(file_descriptor_table[i].flags == FILE_FLAG_UNUSED)
		return &file_descriptor_table[i];
	}

  return NULL; // Out of file descriptors.
}

int16_t FAT_ReadFile(FAT_BPB *bpb, uint16_t fd_id, char *buffer, uint32_t bytes)
{
  FAT_FILE_DESCRIPTOR file_descriptor = file_descriptor_table[fd_id];
  
  if((file_descriptor.flags & FILE_FLAG_READ) != FILE_FLAG_READ)
	return FAT_ERROR_MODE_NOT_READ;

  // What drive is this?
  // Watch out! Need to add toupper() to paths
  int drive_id = (file_descriptor.path[0] - 0x41);

  printf("FAT_ReadFile: OK to read %d bytes of file %s (fd %d)\n",
		 bytes,
		 file_descriptor.path,
		 fd_id);
  char filename[16];
  strcpy(filename, file_descriptor.path + 3);
  
  // TOOD: subdirectories
  printf("FAT_ReadFile: Searching root directory of drive %c for file %s\n",
		 file_descriptor.path[0],
		 filename);
  
  FAT_ROOT_DIRECTORY_ENTRY *root_entry = FAT_SearchRootDirectory(rom_root_dir, filename);
  if(root_entry == NULL) {
	printf("FAT_ReadFile: failed to find file %s in root directory\n", filename);
	return FAT_ERROR_FILE_NOT_FOUND;
  }

  // -1 = read to end

  // OK, where does this file start?
  uint32_t cluster_size = FAT_ClusterSize(bpb);
  printf("FAT_ReadFile: file starts at cluster %d\n", root_entry->first_cluster_low);
  printf("FAT_ReadFile: file is %d bytes (%d clusters)\n",
		 root_entry->file_size,
		 root_entry->file_size / cluster_size + 1);

  int32_t bytes_remaining = bytes;
  int32_t bytes_to_read = 0;
  
  //TODO: generalize for any drive letter
  uint16_t cluster_to_read = root_entry->first_cluster_low;

  while(1)
	{
	  if(bytes_remaining > cluster_size)
		bytes_to_read = cluster_size;
	  else
		bytes_to_read = bytes_remaining;
	  
	  FAT_ReadCluster(drive_id, cluster_to_read-2, buffer, bytes_to_read);
	  buffer += cluster_size;
	  bytes_remaining -= cluster_size;

	  if(bytes_remaining <= 0)
		break;
	  
	  cluster_to_read = rom_fat[cluster_to_read];
	  if((cluster_to_read & 0xFF8) == 0xFF8)
		break;
	}

  return 0;  
}

FAT_ROOT_DIRECTORY_ENTRY *FAT_SearchRootDirectory(FAT_ROOT_DIRECTORY_ENTRY *root_dir,
												 char *filename)
{
  //TODO: fetch BPB of drive and get the actual root dir entry count
  
  char drive_filename[12];
  
  for(int i=0;i<224;i++)
	{
	  memset(drive_filename, 0x00, 12);
	  drive_filename[0] = root_dir[i].allocation_or_first_letter;
	  
	  if(drive_filename[0] == 0x00)
		{
		  printf("FAT: End of directory table reached\n");
		  break; // End of table marker.
		}
	  else if(drive_filename[0] == 0x2E)
		{
		  printf("FAT: skipping dot entry\n");
		  continue; // Dot entry.
		}
	  else if(drive_filename[0] == 0xE5)
		{
		  printf("Skipping deleted file\n");
		  continue; // File is deleted.
		}

	  int current_length = 1;
	  // Construct the filename. Elide the spaces in the FAT's filename.
	  for(int j=0;j<7;j++)
		{
		  if(root_dir[i].filename[j] != 0x20)
			{
			  drive_filename[j+1] = root_dir[i].filename[j];
			  current_length++;
			}
		  else
			{
			  break;
			}
		}

	  drive_filename[current_length++] = '.';

	  for(int j=0;j<3;j++)
		{
		  if(root_dir[i].filename[7+j] != 0x20)
			{
			  drive_filename[current_length++] = root_dir[i].filename[j+8];
			}
		  else
			{
			  break;
			}
		}

	  //printf("Constructed filename: %s\n", drive_filename);
	  if(strcmp(drive_filename, filename))
		{
		  //printf("FAT: No match between '%s' and '%s'\n", drive_filename, filename);
		}
	  else
		{
		  //printf("FAT: Matched '%s' to '%s'\n", drive_filename, filename);
		  return &root_dir[i];
		}
	}

  return NULL;

}

int16_t FAT_ReadCluster(DRIVE_LETTER drive, uint32_t cluster_num, char *buffer, uint32_t bytes)
{
  printf("FAT_ReadCluster: Requested %d bytes of cluster %d from drive %c\n",
		 bytes,
		 cluster_num,
		 FAT_DriveNumberToDriveLetter(drive));
  printf("FAT_ReadCluster: Writing to buffer at %06X\n", buffer);

  FAT_BPB *bpb = drive_bpb[drive];
  
  if(drive == DRIVE_R)
	{
	  // ROM drive, so just read data from the ROM.
	  // Calculate cluster offset.
	  uint32_t data_offset = FAT_OffsetDataStart(bpb);
	  uint32_t cluster_start_offset = FAT_ClusterSize(bpb) * cluster_num;
		  
	  char *data_ptr = (unsigned char *)(ROM_DRIVE_START +
												  data_offset +
												  cluster_start_offset);
	  for(int i=0;i<bytes;i++)
		{
		  buffer[i] = data_ptr[i];
		}

	  printf("Read complete.\n");

	  return FAT_ERROR_NONE;
	}
  else
	{
	  //TODO: Read a sector into memory.
	  printf("Non-ROM storage not yet supported.\n");
	  return FAT_ERROR_FILE_NOT_FOUND;
	}
   
}

/*****************************
 * Disk region calculation.
 *****************************/

uint32_t FAT_OffsetFATStart(FAT_BPB *bpb)
{
  return bpb->bytes_per_sector * bpb->reserved_sectors;
}

uint32_t FAT_OffsetRootDirStart(FAT_BPB *bpb)
{
  return FAT_OffsetFATStart(bpb) +
	(bpb->bytes_per_sector * bpb->sectors_per_fat * bpb->fat_copies);
}

uint32_t FAT_OffsetDataStart(FAT_BPB *bpb)
{
  return FAT_OffsetRootDirStart(bpb) +
	(bpb->root_directory_entries * 32);
}

uint32_t FAT_ClusterSize(FAT_BPB *bpb)
{
  return bpb->sectors_per_cluster * bpb->bytes_per_sector;
}

char FAT_DriveNumberToDriveLetter(int number)
{
  return (char)number+0x41;
}
