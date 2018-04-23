INCDIR = ./include/

CINCDIR = CInclude
AINCDIR = AInclude

all:
	vbccm68k test.c -c99 -I$(INCDIR)/$(CINCDIR)
	vasmm68k_mot test.asm -m68000 -Fvobj -o test.o
	vlink -M -belf32m68k -Bstatic -q -Tsbc.ld test.o -o test.elf
	cp test.elf /mnt/c/EASy68K/test.elf

.PHONY: image

image:
	cp /mnt/c/EASy68K/romfs.ima ../bootrom/src
