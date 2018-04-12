#pragma once

#include "mmio.h"
#include "stdint.h"
#include "serial.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FAT12 constants */
#define FAT12_FAT_SECTOR_COUNT 2
#define FAT12_ROOT_DIR_SECTOR_COUNT 14

typedef struct fat_bpb_t {
 uint16_t bytes_per_sector;
 uint8_t sectors_per_cluster;
  uint16_t reserved_sectors;
  uint8_t fat_copies;
  uint16_t root_directory_entries;
  uint16_t total_sectors;
  uint8_t media_descriptor_type;
  uint16_t sectors_per_fat;
  uint16_t sectors_per_track;
  uint16_t heads;
  uint16_t hidden_sectors;
  uint8_t extended_fields_present;
  char volume_label[12];
char filesystem_type[12];
} FAT_BPB;

typedef struct fat_root_directory_entry_t {
  char allocation_or_first_letter;
  char filename[11];
  uint8_t attributes;
  uint8_t reserved;
  uint8_t creation_time_tenths;
  uint16_t creation_time;
  uint16_t creation_date;
  uint16_t access_date;
  uint16_t first_cluster_high; /* FAT32 only */
  uint16_t modified_time;
  uint16_t modified_date;
  uint16_t first_cluster_low;
  uint32_t file_size;
} FAT_ROOT_DIRECTORY_ENTRY;
\
static FAT_BPB rom_bpb;
static uint16_t rom_fat[5120]; /* TODO: dynamic memory allocation */
static FAT_ROOT_DIRECTORY_ENTRY rom_root_dir[224];

#define SECTOR_OFFSET(data, bytes_per_sector, num) ( (char *)data+(bytes_per_sector*(num)) )

void FAT_BootROM();
void FAT_ReadBPB(FAT_BPB *bpb, char *data);
void FAT_PrintBPBInfo(FAT_BPB *bpb);
void FAT_DecodeFAT12FAT(FAT_BPB, char *sector, uint16_t *decoded_fat);
void FAT_DumpRootDirectory(FAT_BPB, FAT_ROOT_DIRECTORY_ENTRY *dir_list, char *sector);
