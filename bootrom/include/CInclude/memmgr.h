#ifndef MEMMGR_H__
#define MEMMGR_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "procyon.h"

/*
typedef enum memmgr_flags_t {
  MEM_NOFLAGS = 0x00,
  MEM_FIXED = 0x01, // Do not move the underlying memory.
  MEM_ZERO = 0x02,  // Zero when allocating.
} MEMMGR_ALLOC_FLAGS;

typedef enum memmgr_internal_flags_t {
  MEM_I_FIXED = 0x01,   // never move this memory
  MEM_I_INVALID = 0x02, // root handle
} MEMMGR_INTERNAL_FLAGS;

typedef struct memmgr_internal_handle_t {
  struct memmgr_internal_handle_t *next;
  void *points_to;
  uint32_t bytes;
  uint8_t flags;
  uint8_t lock_count;
  uint16_t padding; // make this an even 16 bytes until we need more fields
} MEMMGR_INTERNAL_HANDLE;

extern MEMMGR_INTERNAL_HANDLE *MEMMGR_internal_handles_list;

typedef void* MEMMGR_HANDLE;

void MEMMGR_Initialize();
MEMMGR_HANDLE MEMMGR_AllocMem(uint32_t size, MEMMGR_ALLOC_FLAGS flags);
void MEMMGR_FreeMem(MEMMGR_HANDLE *handle);
void *MEMMGR_LockHandle(MEMMGR_HANDLE *handle);
*/

typedef void * CPTR;
typedef CPTR* HANDLE;

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
