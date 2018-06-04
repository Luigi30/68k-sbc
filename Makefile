.PHONY: all clean symbols

all:
	make -C ./libc
	make -C ./bootrom

clean:
	make -C ./bootrom clean
	make -C ./libc clean

symbols:
	make -C ./bootrom symbols
	@echo "libc symbols are TODO"
