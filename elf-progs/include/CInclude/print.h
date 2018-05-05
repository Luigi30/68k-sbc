#ifndef PRINT_H
#define PRINT_H

typedef char *va_list;

void PrintStringToDevice(__reg("d0") unsigned char device_num, __reg("a0") char *str, __reg("a1") va_list args) = "\tmove.l a1,-(a7)\n \tmove.l a0,-(a7)\n \tmove.l d0,-(a7)\n \tdc.w $A003\n \tadd.l #16,sp\n";

#endif
