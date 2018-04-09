CC = vbccm68k
AS = vasmm68k_mot
LD = vlink

CFLAGS  = -cpu=68000 -c99 -I$(INCDIR)
ASFLAGS = -m68000 -Fvobj

INCDIR = include
OBJDIR = obj
BINDIR = bin
SRCDIR = src

_OBJS = string.o stdio.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

all: $(BINDIR)/libc68k.a
	@echo
	@echo Creating libc68k.a
	@echo

$(OBJDIR)/%.o: $(SRCDIR)/%.S
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJDIR)/%.s: $(SRCDIR)/%.c $(INCDIR)/%.h
	$(CC) $(CFLAGS) -o=$@ $<

$(BINDIR)/libc68k.a: $(OBJS)
	ar rcs $@ $<
