.PHONY: all clean

all:
	make -C ./libc
	make -C ./bootrom

clean:
	make -C ./bootrom clean
	make -C ./libc clean
