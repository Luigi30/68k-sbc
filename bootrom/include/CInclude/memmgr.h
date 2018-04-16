#ifndef MEMMGR_H__
#define MEMMGR_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "procyon.h"

#define MEMMGR_BLOCK_HEADER_SIZE 20
#define SYSTEM_HEAP_SIZE 65536

typedef enum memmgr_block_flags_t {
  MEMMGR_BLOCK_FLAG_NONE   = 0x00,
  MEMMGR_BLOCK_FLAG_FIXED  = 0x01,
  MEMMGR_BLOCK_FLAG_PURGE  = 0x02,
  MEMMGR_BLOCK_FLAG_FREE   = 0x04,
  MEMMGR_BLOCK_FLAG_LOCKED = 0x08,
} MEMMGR_BLOCK_FLAGS;

#define MEMMGR_BLOCK_OFFSET_NEXT     -16
#define MEMMGR_BLOCK_OFFSET_PREVIOUS -12
#define MEMMGR_BLOCK_OFFSET_SIZE     -8
#define MEMMGR_BLOCK_OFFSET_FLAGS    -4

typedef struct memmgr_block_t
{
  struct memmgr_block_t *next;
  struct memmgr_block_t *previous;
  uint32_t size;
  uint32_t flags;
  CPTR destination;
} MEMMGR_BLOCK;

extern MEMMGR_BLOCK *system_heap_blocks;

#define MASTER_POINTER_COUNT 512
extern CPTR master_pointer_list[MASTER_POINTER_COUNT];

void MEMMGR_Init();

HANDLE MEMMGR_NewHandle(uint32_t size);
int MEMMGR_DisposeHandle(HANDLE h);

CPTR MEMMGR_NewPtr(uint32_t requested_size);
int MEMMGR_DisposePtr(CPTR p);

CPTR MEMMGR_AllocateBlock(uint32_t requested_size, MEMMGR_BLOCK_FLAGS flags);
void MEMMGR_FreeBlock(CPTR block);

void MEMMGR_DumpSystemHeapBlocks();
CPTR MEMMGR_GetUnusedMasterPointer();

void *MEMMGR_GetBlockForHandle(HANDLE h);

/* memory manager errors */
#define MEMMGR_ERR_NONE 0
#define MEMMGR_ERR_NULL_HANDLE -1
#define MEMMGR_ERR_BLOCK_IS_FREE -2

#endif