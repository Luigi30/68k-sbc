#include "memmgr.h"

MEMMGR_BLOCK *system_heap_blocks = NULL;

CPTR system_heap_start;
CPTR system_heap_end;

CPTR master_pointer_list[MASTER_POINTER_COUNT];

void MEMMGR_Initialize()
{
  system_heap_start = (CPTR)0x104000;
  system_heap_end = (CPTR)0x114000;

  printf("MEMMGR_Initialize()\n");
  
  // Create a master block at system_heap_start. It's free real estate!
  MEMMGR_BLOCK *master_block = system_heap_start;
  master_block->next = NULL;
  master_block->previous = NULL;
  master_block->size = (uint32_t)system_heap_end - (uint32_t)system_heap_start;
  master_block->flags = MEMMGR_BLOCK_FLAG_FREE;
  master_block->destination = (CPTR)((uint32_t)(system_heap_start)+sizeof(MEMMGR_BLOCK));

  printf("System heap size is %06x\n", master_block->size);
  printf("System heap location: $%06X-$%06X\n", system_heap_start, system_heap_end);

  system_heap_blocks = master_block;

  for(int i=0;i<512;i++)
	{
	  master_pointer_list[i] = 0x0;
	}
}

CPTR MEMMGR_NewPtr(uint32_t requested_size)
{
  printf("MEMMGR_NewPtr: Requested a %d byte block\n", requested_size);
  CPTR block = MEMMGR_AllocateBlock(requested_size, MEMMGR_BLOCK_FLAG_FIXED);

  // Return the block's data area
  return (char *)(block) + 20;
}

int MEMMGR_DisposePtr(CPTR p)
{
  MEMMGR_FreeBlock(p);

  return 0;
}

HANDLE MEMMGR_NewHandle(uint32_t requested_size)
{
  printf("MEMMGR_NewHandle: Requested a %d byte block\n", requested_size);
  CPTR block = MEMMGR_AllocateBlock(requested_size, MEMMGR_BLOCK_FLAG_NONE);
  printf("MEMMGR_NewHandle: Got block $%06X\n", block);
  
  // Add the block to the master pointer list.
  CPTR master = MEMMGR_GetUnusedMasterPointer();
  printf("MEMMGR_NewHandle: Using master pointer at %06X\n", master);
  MMIO32((uint32_t)master) = (uint32_t)block+20;

  // Pointer to the master pointer list entry is a handle.
  return master; 
}

CPTR MEMMGR_AllocateBlock(uint32_t requested_size, MEMMGR_BLOCK_FLAGS flags)
{
  uint32_t adjusted_size = (requested_size + MEMMGR_BLOCK_HEADER_SIZE + 4) & 0xFFFFFFF0;

  MEMMGR_BLOCK *block = system_heap_blocks;

  // Find a free block that's big enough.
  while(block != NULL)
	{
	  printf("block at $%06X: points to %06X, %d bytes. free? %d\n",
			 block,
			 block->destination,
			 block->size,
			 (block->flags & MEMMGR_BLOCK_FLAG_FREE) == 0x04);

	  if((uint32_t)block->previous > RAMEnd ||
		 (uint32_t)block->next > RAMEnd ||
		 (uint32_t)block->destination > RAMEnd)
		{
		  printf("MEMMGR_AllocateBlock: Heap is corrupt! \nHalting system.\n");
		  while(true) {};
		}

	  if((uint32_t)block->destination == 0x0)
		{
		  printf("MEMMGR_AllocateBlock: Heap is corrupt: block points to 0x000000.\nHalting system.\n");
		  while(true) {};
		}
			 
	  if((block->flags & MEMMGR_BLOCK_FLAG_FREE) &&
		 (block->size >= adjusted_size))
		{

		  // We found a suitable free block!

		  // Create a new block that is a resized version of the old block.
		  MEMMGR_BLOCK *new_block = (MEMMGR_BLOCK *)(((char *)block)+adjusted_size);
		  new_block->size = block->size - adjusted_size;
		  if(new_block->next != NULL)
			new_block->next->previous = new_block;
		  new_block->previous = block;
		  new_block->flags = MEMMGR_BLOCK_FLAG_FREE;
		  new_block->destination = (CPTR)((uint32_t)(new_block)+20);
		  
		  block->next = new_block;
		  block->size = adjusted_size;
		  block->flags = flags;

		  MEMMGR_DumpSystemHeapBlocks();
		  return block;
		  
		}
	  else {
		block = block->next;
	  }
	}

  // No free block big enough.
  printf("MEMMGR: Failed to find free block of %d bytes\n", adjusted_size);
  return NULL;
}

int MEMMGR_DisposeHandle(HANDLE h)
{
  long *block = (long *)(((char *)*h) - 20);
  printf("MEMMGR_DisposeHandle: freeing block at $%06X. block starts at $%06X\n", *h, block);
  block[3] = MEMMGR_BLOCK_FLAG_FREE;
  
  return 0;
}

void MEMMGR_FreeBlock(CPTR block)
{
  ((long *)block)[3] = MEMMGR_BLOCK_FLAG_FREE;
}

void MEMMGR_DumpSystemHeapBlocks()
{
  MEMMGR_BLOCK *block = system_heap_blocks;

  printf("*** DumpSystemHeapBlocks ***\n");
  while(block != NULL)
	{
	  printf("BLOCK AT $%06X - size %d, ptr: $%06X, fixed: %d, free: %d\n",
			 block,
			 block->size,
			 block->destination,
			 (block->flags & MEMMGR_BLOCK_FLAG_FIXED) == MEMMGR_BLOCK_FLAG_FIXED,
			 (block->flags & MEMMGR_BLOCK_FLAG_FREE) == MEMMGR_BLOCK_FLAG_FREE);
	  block = block->next;
	}

  printf("*** DumpSystemHeapBlocks end ***\n");
}

CPTR MEMMGR_GetUnusedMasterPointer()
{
  printf("MEMMGR: Master pointer list is $%06X, searching for unused master pointer\n",
		 master_pointer_list);
  for(int i=0;i<MASTER_POINTER_COUNT;i++)
	{
	  if(master_pointer_list[i] == NULL)
		return &(master_pointer_list[i]);
	}
}

int MEMMGR_LockHandle(HANDLE h)
{
  // Is this handle valid?
  if(*h == NULL)
	return MEMMGR_ERR_NULL_HANDLE;

  long *block = (long *)MEMMGR_GetBlockForHandle(h);

  // Is this a handle to a freed block?
  if((block[3] & MEMMGR_BLOCK_FLAG_FREE) == MEMMGR_BLOCK_FLAG_FREE)
	return MEMMGR_ERR_BLOCK_IS_FREE;

  block[3] |= MEMMGR_BLOCK_FLAG_LOCKED; // Lock the handle.

  return MEMMGR_ERR_NONE;
}

int MEMMGR_UnlockHandle(HANDLE h)
{
  // Is this handle valid?
  if(*h == NULL)
	return MEMMGR_ERR_NULL_HANDLE;

  long *block = (long *)MEMMGR_GetBlockForHandle(h);

  // Is this a handle to a freed block?
  if((block[3] & MEMMGR_BLOCK_FLAG_FREE) == MEMMGR_BLOCK_FLAG_FREE)
	return MEMMGR_ERR_BLOCK_IS_FREE;

  block[3] = (block[3] & ~MEMMGR_BLOCK_FLAG_LOCKED); // Unlock if locked.
}

void * MEMMGR_GetBlockForHandle(HANDLE h)
{
  return (void *)(((char *)*h) - 20);
}
