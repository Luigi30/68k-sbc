CC = vbccm68k
AS = vasmm68k_mot
LD = vlink

CFLAGS  = -cpu=68000 -c99 -I$(INCDIR) -I$(COMMONDIR)/$(CINCLUDE)
ASFLAGS = -m68000 -Fvobj -I$(COMMONDIR)/$(AINCLUDE)

INCDIR = include
OBJDIR = obj
BINDIR = bin
SRCDIR = src
COMMONDIR = ../common

CINCLUDE = CInclude
AINCLUDE = AInclude

_OBJS = string.o stdio.o math32.o stdlib.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

all: $(BINDIR)/libc68k.a
	@echo
	@echo Creating libc68k.a
	@echo

$(OBJDIR)/%.o: $(SRCDIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJDIR)/%.s: $(SRCDIR)/%.c $(INCDIR)/%.h
	$(CC) $(CFLAGS) -o=$@ $<

$(BINDIR)/libc68k.a: $(OBJS)
	m68k-elf-ar rcs $@ $^

.PHONY: clean

clean:
	rm -f obj/* bin/*
