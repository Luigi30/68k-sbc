#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "devices/devices.h"

void PrintStringToDevice(__reg("d0") uint8_t device_num, __reg("a0") char *str, __reg("a1") va_list args);

#endif
