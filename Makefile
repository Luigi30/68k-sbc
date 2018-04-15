AS      = vasmm68k_mot
ASFLAGS = -m68000 -align -I$(INCDIR)

CC	= vbccm68k
CFLAGS	= -cpu=68000 -c99 -I$(LIBCINCDIR)

LD=vlink
LDFLAGS = -M -Bstatic -bsrec28 -nostdlib -Tsbc.ld -Llibc/bin -Lc68k

SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
INCDIR = .
LIBCINCDIR = ./libc/include

$(BINDIR)/bootrom.s68: $(OBJDIR)/crt0.o $(OBJDIR)/bootrom.o $(OBJDIR)/monitor.o $(OBJDIR)/romfs.o $(OBJDIR)/fat.o $(OBJDIR)/log.o \
		       	$(OBJDIR)/memmgr.o libc/bin/libc68k.a
	vlink $(LDFLAGS) -lc68k -o $(BINDIR)/bootrom.s68 $^
	cp bin/bootrom.s68 /mnt/c/EASy68K/
	m68k-atari-mint-objcopy -I srec -O binary $@ --pad-to=0x100000 /mnt/c/msys64/home/Luigi/mame/roms/luigisbc/68000.bin

$(OBJDIR)/%.o: $(SRCDIR)/%.s
	$(AS) $(ASFLAGS) -Fvobj -o $@ $<

$(OBJDIR)/bootrom.o: $(SRCDIR)/bootrom.s
	$(AS) $(ASFLAGS) -Fvobj -o $@ $<

$(OBJDIR)/monitor.s: $(SRCDIR)/monitor.c
	$(CC) $(CFLAGS) -o=$@ $<

$(OBJDIR)/monitor.o: $(OBJDIR)/monitor.s
	$(AS) $(ASFLAGS) -Fvobj -o $@ $<

$(OBJDIR)/fat.s: $(SRCDIR)/fat.c
	$(CC) $(CFLAGS) -o=$@ $<

$(OBJDIR)/fat.o: $(OBJDIR)/fat.s
	$(AS) $(ASFLAGS) -Fvobj -o $@ $<

$(OBJDIR)/log.s: $(SRCDIR)/log.c
	$(CC) $(CFLAGS) -o=$@ $<

$(OBJDIR)/log.o: $(OBJDIR)/log.s
	$(AS) $(ASFLAGS) -Fvobj -o $@ $<

$(OBJDIR)/memmgr.s: $(SRCDIR)/memmgr.c
	$(CC) $(CFLAGS) -o=$@ $<

$(OBJDIR)/memmgr.o: $(OBJDIR)/memmgr.s
	$(AS) $(ASFLAGS) -Fvobj -o $@ $<

$(OBJDIR)/romfs.o: $(SRCDIR)/romfs.s
	$(AS) $(ASFLAGS) -Fvobj -o $@ $<

###############

.PHONY: clean run libc all

all:	libc $(BINDIR)/bootrom.s68

libc:
	$(MAKE) -C libc

clean:
	rm obj/*.o obj/*.s bin/*

run:
	/mnt/c/msys64/home/Luigi/mame/mame64.exe luigisbc -debug -window -rompath /mnt/c/msys64/home/Luigi/mame/roms
