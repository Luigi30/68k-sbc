AS      = vasmm68k_mot
ASFLAGS = -m68000 -align -I$(INCDIR)

CC	= vbccm68k
CFLAGS	= -cpu=68000 -c99 -I$(LIBCINCDIR)

LD=vlink
LDFLAGS = -M -Bstatic -bsrec28 -nostdlib -Tsbc.ld -Llibc/bin

SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
INCDIR = .
LIBCINCDIR = ./libc/include

$(BINDIR)/bootrom.s68: $(OBJDIR)/crt0.o $(OBJDIR)/bootrom.o $(OBJDIR)/monitor.o
	vlink $(LDFLAGS) -l c68k -o $(BINDIR)/bootrom.s68 $^
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

libc/bin/libc68k.a:
	$(MAKE) -C libc

.PHONY: clean

clean:
	rm obj/*.o obj/*.s bin/*
