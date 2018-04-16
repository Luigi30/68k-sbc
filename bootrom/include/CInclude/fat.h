#pragma once

#include "procyon.h"
#include "stdint.h"
#include "serial.h"
#include "log.h"
#include "memmgr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROM_DRIVE_START 0x80000

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
  char filename[12]; // not the filename on the disk! modified!
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

typedef enum drive_letter_t {
  DRIVE_A, DRIVE_B, DRIVE_C, DRIVE_D, DRIVE_E,
  DRIVE_F, DRIVE_G, DRIVE_H, DRIVE_I, DRIVE_J,
  DRIVE_K, DRIVE_L, DRIVE_M, DRIVE_N, DRIVE_O,
  DRIVE_P, DRIVE_Q, DRIVE_R, DRIVE_S, DRIVE_T,
  DRIVE_U, DRIVE_V, DRIVE_W, DRIVE_X, DRIVE_Y,
  DRIVE_Z
} DRIVE_LETTER;

/* file flags */
typedef enum fat_file_flags_t {
  FILE_FLAG_UNUSED = 0x00,
  FILE_FLAG_READ   = 0x01,
  FILE_FLAG_WRITE  = 0x02,
  FILE_FLAG_READWRITE = 0x03,
} FAT_FILE_FLAGS;

typedef struct fat_file_descriptor_t {
  uint16_t id;
  FAT_FILE_FLAGS flags;
  FAT_ROOT_DIRECTORY_ENTRY *root_dir_entry;
  char path[128];
} FAT_FILE_DESCRIPTOR;

#define FILES_LIMIT 10
static FAT_FILE_DESCRIPTOR file_descriptor_table[FILES_LIMIT];
static uint16_t rom_fat[5120]; /* TODO: dynamic memory allocation */
static FAT_ROOT_DIRECTORY_ENTRY rom_root_dir[224];

static FAT_BPB *drive_bpb[26];
static FAT_ROOT_DIRECTORY_ENTRY *drive_root_dir[26];
static uint16_t *drive_fat[26];

#define SECTOR_OFFSET(data, bytes_per_sector, num) ( (char *)data+(bytes_per_sector*(num)) )

void FAT_BootROM();
void FAT_ReadBPB(FAT_BPB *bpb, DRIVE_LETTER drive, char *data);
void FAT_PrintBPBInfo(DRIVE_LETTER drive);
void FAT_DecodeFAT12FAT(char *sector, uint16_t *decoded_fat);
void FAT_DumpRootDirectory(DRIVE_LETTER drive, FAT_ROOT_DIRECTORY_ENTRY *dir_list, char *sector);

FAT_ROOT_DIRECTORY_ENTRY *FAT_SearchRootDirectory(FAT_ROOT_DIRECTORY_ENTRY *root_dir, char *filename);

/* Disk region calculation. */
uint32_t FAT_OffsetFATStart(FAT_BPB *bpb);
uint32_t FAT_OffsetRootDirStart(FAT_BPB *bpb);
uint32_t FAT_OffsetDataStart(FAT_BPB *bpb);
uint32_t FAT_ClusterSize(FAT_BPB *bpb);

/* File manipulation routines. */
uint16_t FAT_OpenFile(char *path, FAT_FILE_FLAGS mode);
int16_t FAT_ReadFile(FAT_BPB *bpb, uint16_t fd_id, char *buffer, uint32_t bytes);
int16_t FAT_ReadCluster(DRIVE_LETTER drive, uint32_t cluster_num, char *buffer, uint32_t bytes);
FAT_FILE_DESCRIPTOR * FAT_GetUnusedFileDescriptor();

char FAT_DriveNumberToDriveLetter(int number);

/* FAT error codes */
#define FAT_ERROR_NONE               0
#define FAT_ERROR_INVALID_FILE_MODE -1
#define FAT_ERROR_FILE_NOT_OPEN     -2
#define FAT_ERROR_MODE_NOT_READ     -3
#define FAT_ERROR_MODE_NOT_WRITE    -4

//ReadFile errors
#define FAT_ERROR_FILE_NOT_FOUND    -5
