#include "stdlib.h"
#include "stdio.h"

#define MMIO32(x)   (*(volatile unsigned long *)(x))
#define MMIO16(x)   (*(volatile unsigned short *)(x))
#define MMIO8(x)    (*(volatile unsigned char *)(x))

void swap(char *a, char *b)
{
  char c = *a;
  *a = *b;
  *b = c;
}

void reverse(char *str, int length)
{
  int start = 0;
  int end = length - 1;
  while(start < end)
	{
	  swap((str+start), (str+end));
	  start++;
	  end--;
	}
}

int atoi(char *str)
{
  int result = 0;
  int sign = 1;
  int i=0;

  if(str[0] == '-')
	{
	  sign = -1;
	  i++;
	}

  for(; str[i] != '\0'; ++i)
	result = result*10 + str[i] - '0';

  return result*sign;
}

char *itoa(int num, char *str, int base)
{
  int i=0;
  int isNegative = false;

  if(num == 0)
	{
	  str[i++] = '0';
	  str[i] = '\0';
	  return str;
	}

  if(num < 0 && base == 10)
	{
	  isNegative = 1;
	  num = -num;
	}

  while(num != 0)
	{
	  int rem = num % base;
	  str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
	  num = num/base;
	}

  if(isNegative)
	str[i++] = '-';

  str[i] = '\0';

  reverse(str, i);

  return str;
}

/* malloc: here be dragons */
#define BLOCK_SIZE 16

uint32_t first_free_block = 0;

/* Straight fixed-block allocator suitable for single-tasking. */
void *malloc(int size)
{	
  int requested_blocks = size/BLOCK_SIZE;

  if(size % BLOCK_SIZE != 0) //Allocate another block for the leftovers.
	requested_blocks++;

  // Grab the start of the global heap.
  char *current_heap_bottom = (char *)(MMIO32(P_MEMBOT));
  // Reposition the pointer to point to the first free block.
  current_heap_bottom += first_free_block * BLOCK_SIZE;
  
  // Update the first free block.
  first_free_block += requested_blocks;

  printf("malloc: %d bytes (%d blocks) requested. returning: $%06X\n", size, requested_blocks, current_heap_bottom);
  return current_heap_bottom;
}

void free(void *block)
{
  /* TODO: free() */
}

