#include <string.h>

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
