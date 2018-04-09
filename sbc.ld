ENTRY(_bios_start)

MEMORY
{
	ROM (rx) : ORIGIN = 0x000000, LENGTH = 1M
	RAM (wx) : ORIGIN = 0x100000, LENGTH = 1M
}

SECTIONS
{
	TEXT 0x000000 : {
		crt0.o( CODE )
		. = 0x1000;
		bootrom.o( CODE )
		test.o( CODE )
		monitor.o( CODE )
		libc68k.a ( CODE )
		_DATA_REAL = .;
	}
	DATA 0x100000 : AT (0x80000) { 
		_DATA_START = .;
		*(.data DATA data)
		_DATA_END = .;
	}
	BSS 0x100000 + SIZEOF(DATA) : { 
		_BSS_START = .;
		*(.bss BSS bss)
		*(COMMON)
		_BSS_END = .; 
	}
}
