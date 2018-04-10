#include "fat.h"

#define ROMFS_START 0x80000

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

void FAT_BootROM()
{
  /* Identify the disk at ROMFS_START. */
  int bootstrap = (MMIO32(ROMFS_START) >> 8)

  char buffer[64];
  itoa(bootstrap, buffer, 16);

  serial_string_out("\nFAT12: Bootstrap is ");
  serial_string_out(buffer);
  serial_string_out("\n");

  strncpy(buffer, (const char *)(ROMFS_START+3), 8);
  serial_string_out("FAT12: OEM Name is ");
  serial_string_out(buffer);
  serial_string_out("\n");
}
