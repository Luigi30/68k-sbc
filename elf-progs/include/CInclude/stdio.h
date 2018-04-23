#ifndef STDIO_H
#define STDIO_H

/* Example: printf(char *)
 * This expands to
 * move.l a0,-(a7) ; pushes the parameter onto the stack
 * dc.w   $A002    ; performs OSFunc $A002
 * add.l  #10,sp    ; corrects the stack by adding (parameters size)+4
 */
void printf(__reg("a0") char *) = "\tmove.l a0,-(a7)\n\tdc.w $A002\n\tadd.l #8,sp\n"

#endif
