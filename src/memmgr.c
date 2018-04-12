#include "memmgr.h"

MEMMGR_INTERNAL_HANDLE *MEMMGR_internal_handles_list;

MEMMGR_HANDLE MEMMGR_AllocMem(uint32_t size, MEMMGR_ALLOC_FLAGS flags)
{
  //find the end of the handle list
  MEMMGR_INTERNAL_HANDLE *current = MEMMGR_internal_handles_list;
  MEMMGR_INTERNAL_HANDLE *previous = NULL;
  
  while(current != NULL)
	{
	  previous = current;
	  current = current->next;
	}

  current = malloc(sizeof(MEMMGR_INTERNAL_HANDLE)); //malloc a handle
  previous->next = current; //attach it to the list

  current->points_to = malloc(size);
  current->bytes = size;

  //Handle flags.
  if(MEM_FIXED)
	current->flags = MEM_I_FIXED;
  if(MEM_ZERO)
	memset(current->points_to, 0x00, size);

  current->lock_count = 0;

  return current;
}

void MEMMGR_FreeMem(MEMMGR_HANDLE *handle)
{
  /* TODO */
}

void *MEMMGR_LockHandle(MEMMGR_HANDLE *handle)
{
  // Increment the lock count and return a pointer.
  ((MEMMGR_INTERNAL_HANDLE *)handle)->lock_count++;
  return ((MEMMGR_INTERNAL_HANDLE *)handle)->points_to;
}

void MEMMGR_Initialize()
{
  MEMMGR_INTERNAL_HANDLE *root = malloc(sizeof(MEMMGR_INTERNAL_HANDLE));

  root->next = NULL;
  root->points_to = NULL;
  root->bytes = 0;
  root->flags = MEM_I_INVALID | MEM_I_FIXED;
  root->lock_count = 0;
}
