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
		*.o( CODE )
		*.a( CODE)
		. = ALIGN(4);
		_DATA_REAL = .;
	} >ROM
	ROM 0x80000 : {
		romfs.o( ROMFS )
	} >ROM
	DATA: { 
		VBCC_CONSTRUCTORS
		*(.data DATA data)
		. = ALIGN(4);
	} >RAM AT>ROM
	BSS (NOLOAD): { 
		. = ALIGN(0x1000);
		_BSS_START = .;
		*(.bss BSS bss)
		*(COMMON)
		. = ALIGN(4);
		_BSS_END = .;
	} >RAM

	_DATA_START = LOADADDR(DATA);
	_DATA_END = LOADADDR(DATA) + SIZEOF(DATA);
}
