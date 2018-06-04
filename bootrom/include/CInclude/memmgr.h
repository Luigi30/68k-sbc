#ifndef MEMMGR_H__
#define MEMMGR_H__

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "procyon.h"

#define MEMMGR_BLOCK_HEADER_SIZE 20
#define SYSTEM_HEAP_SIZE 65536
#define APPLICATION_HEAP_SIZE 131072

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

#define MASTER_POINTER_COUNT 256

typedef struct memmgr_block_t
{
  struct memmgr_block_t *next;
  struct memmgr_block_t *previous;
  uint32_t size;
  uint32_t flags;
  CPTR destination;
} MEMMGR_BLOCK;

typedef struct memmgr_heap_t
{
  struct memmgr_heap_t *next;
  struct memmgr_heap_t *previous;
  CPTR *master_pointers;
  uint32_t size; 
  MEMMGR_BLOCK *blocks;
} Heap;

extern Heap heap_system; // MacOS: SysZone
extern Heap heap_application; // MacOS: ApplZone
extern Heap active_heap; // MacOS: TheZone

typedef enum {
  H_CURHEAP = 0x00,
  H_SYSHEAP = 0x01,
  H_CLEAR   = 0x02,
} MEMMGR_MALLOC_FLAGS;

void MEMMGR_Init();

HANDLE MEMMGR_NewHandle(uint32_t size, MEMMGR_MALLOC_FLAGS flags);
int MEMMGR_DisposeHandle(HANDLE h);

CPTR MEMMGR_NewPtr(uint32_t requested_size, MEMMGR_MALLOC_FLAGS flags);
int MEMMGR_DisposePtr(CPTR p);

// Internal functions.
CPTR MEMMGR_AllocateBlock(Heap *heap, uint32_t requested_size, MEMMGR_BLOCK_FLAGS flags);
void MEMMGR_FreeBlock(CPTR block);

void *MEMMGR_GetBlockForHandle(HANDLE h);
void MEMMGR_CombineFreeBlocks(Heap *heap);


// Debugging functions.
void MEMMGR_DumpHeapBlocks(Heap *heap);
CPTR MEMMGR_GetUnusedMasterPointer(Heap *heap);

/* memory manager errors */
#define MEMMGR_ERR_NONE 0
#define MEMMGR_ERR_NULL_HANDLE -1
#define MEMMGR_ERR_BLOCK_IS_FREE -2

#endif
