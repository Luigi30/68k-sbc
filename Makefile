AS      = vasmm68k_mot
ASFLAGS = -m68000 -align -I$(INCDIR)

CC	= vbccm68k
CFLAGS	= -cpu=68000 -c99

LD=vlink
LDFLAGS = -M -brawbin1 -nostdlib -Tsbc.ld

SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
INCDIR = .

$(BINDIR)/bootrom.bin: $(OBJDIR)/crt0.o $(OBJDIR)/bootrom.o $(OBJDIR)/test.o
	vlink $(LDFLAGS) -o $(BINDIR)/bootrom.bin $^
	cp $@ /mnt/c/msys64/home/Luigi/mame/roms/luigisbc/68000.bin
#	m68k-elf-objcopy.exe --pad-to=0x100000 --gap-fill=0xFF bin\\bootrom.bin /mnt/c/msys64/home/Luigi/mame/roms/luigisbc/68000.bin

$(OBJDIR)/crt0.o: $(SRCDIR)/crt0.s
	$(AS) $(ASFLAGS) -Fvobj -o $@ $<

$(OBJDIR)/bootrom.o: $(SRCDIR)/bootrom.s
	$(AS) $(ASFLAGS) -Fvobj -o $@ $<

$(OBJDIR)/test.s: $(SRCDIR)/test.c
	$(CC) $(CFLAGS) -o=$@ $<

$(OBJDIR)/test.o: $(OBJDIR)/test.s
	$(AS) $(ASFLAGS) -Fvobj -o $@ $<

.PHONY: clean

clean:
	rm obj/*.o obj/*.s bin/*
