#include "print.h"

void PrintStringToDevice(uint8_t device_num, char *str, va_list args)
{
  switch(device_num)
	{
	case 0:
	  printf(str, args);
	  break;
	default:
	  printf(str, args);
	  break;
	}
}
