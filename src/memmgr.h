#ifndef MEMMGR_H__
#define MEMMGR_H__

#include <string.h>
#include <stdlib.h>

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

#endif
