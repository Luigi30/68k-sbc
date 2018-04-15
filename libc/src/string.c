#include <string.h>

void *memcpy(void *destination, const void *source, size_t num)
{
  char *source_c = source;
  char *destination_c = destination;
  
  /* TODO: optimize */
  for(int i=0; i<num; i++)
	{
	  destination_c[i] = source_c[i];
	}

  return destination;
}

void *memset(void *destination, int c, size_t num)
{
  unsigned char _c = (unsigned char)c;
  char *destination_c = destination;
  
  for(int i=0; i<num; i++)
	{
	  destination_c[i] = _c;
	}

  return destination;
}

int strcmp(const char *lhs, const char *rhs)
{
  char result;
  int current = 0;
  
  while(1)
	{
	  result = (uint8_t)lhs[current] - (uint8_t)rhs[current];

	  if(result > 0)
		return 1;
	  else if(result < 0)
		return -1;
	  else if((uint8_t)lhs[current] == 0x00 && (uint8_t)rhs[current] == 0x00)
		return 0;

	  current++;
	}
}

size_t strlen(const char *str){
	size_t size = 0;

	while(*str != 0x00){
		str++;
		size++;
	}

	return size;
}

char *strcpy(char *dest, const char *src)
{
  size_t i;

  for(i = 0; src[i] != '\0'; i++)
	{
	  dest[i] = src[i];
	}
  dest[i] = '\0';

  return dest;
}

char *strncpy(char *dest, const char *src, size_t n)
{
  size_t i;

  for(i = 0; i < n && src[i] != '\0'; i++)
	{
	  dest[i] = src[i];
	}
  for(; i<n; i++)
	{
	  dest[i] = '\0';
	}

  return dest;
}
