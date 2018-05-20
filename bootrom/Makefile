AS      = vasmm68k_mot
ASFLAGS = -m68000 -align -I$(COMMONDIR)/AInclude -I$(INCDIR)/AInclude

CC	= vbccm68k
CFLAGS	= -cpu=68000 -c99 -I$(LIBCINCDIR) -I$(INCDIR)/$(CINCLUDE) -I$(COMMONDIR)/$(CINCLUDE)

LD=vlink
LDFLAGS = -M -Bstatic -bsrec28 -nostdlib -Tsbc.ld -L../libc/bin -Lc68k

SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
INCDIR = ./include
LIBCINCDIR = ../libc/include
COMMONDIR = ../common

CINCLUDE = CInclude
AINCLUDE = AInclude

$(BINDIR)/bootrom.s68:	$(OBJDIR)/crt0.o $(OBJDIR)/sysvars.o $(OBJDIR)/bootrom.o $(OBJDIR)/monitor.o $(OBJDIR)/romfs.o $(OBJDIR)/fat.o $(OBJDIR)/log.o \
				    	$(OBJDIR)/memmgr.o $(OBJDIR)/dispatch.o $(OBJDIR)/osfunc.o $(OBJDIR)/elf.o $(OBJDIR)/print.o $(OBJDIR)/vga.o \
						$(OBJDIR)/fastdraw.o $(OBJDIR)/fastdraw_asm.o $(OBJDIR)/vga_asm.o $(OBJDIR)/fastdraw/linedraw.o \
						$(OBJDIR)/vgaconsole.o $(OBJDIR)/mouse.o $(OBJDIR)/mouse_asm.o $(OBJDIR)/nodelist.o $(OBJDIR)/nodelist_asm.o \
						 $(OBJDIR)/devices/devices.o $(OBJDIR)/devices/mouse.o $(OBJDIR)/gfx/gfx.o ../libc/bin/libc68k.a

	vlink $(LDFLAGS) -lc68k -o $(BINDIR)/bootrom.s68 $^
	cp bin/bootrom.s68 /mnt/c/EASy68K/
	m68k-atari-mint-objcopy -I srec -O binary $@ --pad-to=0x100000 /mnt/c/msys64/home/Luigi/mame/roms/luigisbc/68000.bin

# Assemble any ASM source we already have.
$(OBJDIR)/%.o: $(SRCDIR)/%.s
	$(AS) $(ASFLAGS) -Fvobj -o $@ $<

# Compile the C files then assemble the output.
$(OBJDIR)/%.s: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o=$@ $<

$(OBJDIR)/%.o: $(OBJDIR)/%.s
	$(AS) $(ASFLAGS) -Fvobj -o $@ $<

###############

.PHONY: clean run libc all

all:	libc $(BINDIR)/bootrom.s68

libc:
	$(MAKE) -C libc

clean:
	rm -f obj/*.o obj/*.s bin/* obj/gfx/*.o

run:
	/mnt/c/msys64/home/Luigi/mame/mame64.exe luigisbc -debug -window -rompath /mnt/c/msys64/home/Luigi/mame/roms
