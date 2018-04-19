#include "memmgr.h"

Heap heap_system;
Heap heap_application;

CPTR system_pointer_list[MASTER_POINTER_COUNT];
CPTR application_pointer_list[MASTER_POINTER_COUNT];

void MEMMGR_Initialize()
{
  SysHeap = 0x104000;
  SysHeapEnd = 0x114000;

  ApplHeap = 0x1C0000;
  ApplHeapEnd = 0x1FFF00;

  printf("MEMMGR_Initialize()\n");
  
  // Create a master block at system_heap_start. It's free real estate!
  MEMMGR_BLOCK *system_master_block = (MEMMGR_BLOCK *)SysHeap;
  system_master_block->next = NULL;
  system_master_block->previous = NULL;
  system_master_block->size = (uint32_t)SysHeapEnd - (uint32_t)SysHeap;
  system_master_block->flags = MEMMGR_BLOCK_FLAG_FREE;
  system_master_block->destination = (CPTR)((uint32_t)(SysHeap)+sizeof(MEMMGR_BLOCK));
  heap_system.next = NULL;
  heap_system.previous = NULL;
  heap_system.master_pointers = system_pointer_list;
  heap_system.size = (uint32_t)SysHeapEnd - (uint32_t)SysHeap;
  heap_system.blocks = system_master_block;  

  printf("System heap size is %06x\n", system_master_block->size);
  printf("System heap location: $%06X-$%06X\n", SysHeap, SysHeapEnd);

  // Application heap
  MEMMGR_BLOCK *application_master_block = (MEMMGR_BLOCK *)ApplHeap;
  application_master_block->next = NULL;
  application_master_block->previous = NULL;
  application_master_block->size = (uint32_t)ApplHeapEnd - (uint32_t)ApplHeap;
  application_master_block->flags = MEMMGR_BLOCK_FLAG_FREE;
  application_master_block->destination = (CPTR)((uint32_t)(ApplHeap)+sizeof(MEMMGR_BLOCK));
  heap_application.next = NULL;
  heap_application.previous = NULL;
  heap_application.master_pointers = application_pointer_list;
  heap_application.size = (uint32_t)ApplHeapEnd - (uint32_t)ApplHeap;
  heap_application.blocks = application_master_block;

  for(int i=0;i<MASTER_POINTER_COUNT;i++)
	{
	  heap_system.master_pointers[i] = NULL;
	  heap_application.master_pointers[i] = NULL;
	}
  
}

CPTR MEMMGR_NewPtr(Heap *heap, uint32_t requested_size)
{
  printf("MEMMGR_NewPtr: Requested a %d byte block\n", requested_size);
  CPTR block = MEMMGR_AllocateBlock(heap, requested_size, MEMMGR_BLOCK_FLAG_FIXED);

  // Return the block's data area
  return (char *)(block) + 20;
}

int MEMMGR_DisposePtr(Heap *heap, CPTR p)
{
  MEMMGR_FreeBlock(heap, p);

  return 0;
}

HANDLE MEMMGR_NewHandle(Heap *heap, uint32_t requested_size)
{
  printf("MEMMGR_NewHandle: Requested a %d byte block\n", requested_size);
  CPTR block = MEMMGR_AllocateBlock(heap, requested_size, MEMMGR_BLOCK_FLAG_NONE);
  //printf("MEMMGR_NewHandle: Got block $%06X\n", block);
  
  // Add the block to the master pointer list.
  CPTR master = MEMMGR_GetUnusedMasterPointer(heap);
  //printf("MEMMGR_NewHandle: Using master pointer at %06X\n", master);
  MMIO32((uint32_t)master) = (uint32_t)block+20;

  // Pointer to the master pointer list entry is a handle.
  return master; 
}

CPTR MEMMGR_AllocateBlock(Heap *heap, uint32_t requested_size, MEMMGR_BLOCK_FLAGS flags)
{
  uint32_t adjusted_size = (requested_size + MEMMGR_BLOCK_HEADER_SIZE + 4) & 0xFFFFFFF0;

  MEMMGR_BLOCK *block = heap->blocks;

  // Find a free block that's big enough.
  while(block != NULL)
	{
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

		  MEMMGR_DumpHeapBlocks(&heap_system);
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

int MEMMGR_DisposeHandle(Heap *heap, HANDLE h)
{
  //TODO: check if address is actually inside the heap
  uint32_t *block = (uint32_t *)(((uint8_t *)*h) - 20);
  printf("MEMMGR_DisposeHandle: freeing block at $%06X. block starts at $%06X\n", *h, block);
  block[3] = MEMMGR_BLOCK_FLAG_FREE;
  
  return 0;
}

void MEMMGR_FreeBlock(Heap *heap, CPTR block)
{
  //TODO: check if the address is actually inside the heap
  ((uint32_t *)block)[3] = MEMMGR_BLOCK_FLAG_FREE;
}

void MEMMGR_DumpHeapBlocks(Heap *heap)
{
  MEMMGR_BLOCK *block = heap->blocks;

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

CPTR MEMMGR_GetUnusedMasterPointer(Heap *heap)
{
   for(int i=0;i<MASTER_POINTER_COUNT;i++)
	{
	  if(heap->master_pointers[i] == NULL)
		return &(heap->master_pointers[i]);
	}
}

int MEMMGR_LockHandle(HANDLE h)
{
  // Is this handle valid?
  if(*h == NULL)
	return MEMMGR_ERR_NULL_HANDLE;

  uint32_t *block = (uint32_t *)MEMMGR_GetBlockForHandle(h);

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

  int32_t *block = (int32_t *)MEMMGR_GetBlockForHandle(h);

  // Is this a handle to a freed block?
  if((block[3] & MEMMGR_BLOCK_FLAG_FREE) == MEMMGR_BLOCK_FLAG_FREE)
	return MEMMGR_ERR_BLOCK_IS_FREE;

  block[3] = (block[3] & ~MEMMGR_BLOCK_FLAG_LOCKED); // Unlock if locked.
}

void * MEMMGR_GetBlockForHandle(HANDLE h)
{
  return (void *)(((char *)*h) - 20);
}

void MEMMGR_CombineFreeBlocks(Heap *heap)
{
  printf("MEMMGR: Looking for adjacent free blocks.\n");

  MEMMGR_BLOCK *block = heap->blocks;

  while(block != NULL)
	{
	  if((block->flags & MEMMGR_BLOCK_FLAG_FREE) == MEMMGR_BLOCK_FLAG_FREE)
		{
		  //This is a free block. Is the next block free?
		  if((block->next->flags & MEMMGR_BLOCK_FLAG_FREE) == MEMMGR_BLOCK_FLAG_FREE)
			{
			  //Yes. Combine the two blocks, then invalidate any handles to the disposed block.
			  printf("MEMMGR: Combining blocks $%06X and $%06X\n",
					 block,
					 block->next);
			  block->size = block->size + block->next->size;

			  //Remove the next block from the list.
			  block->next = block->next->next;
			  if(block->next != NULL)
				block->next->previous = block;
			}
		}
	  
	  block = block->next;
	}
}
