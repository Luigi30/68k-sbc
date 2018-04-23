ENTRY(__main)

MEMORY
{
	ROM (rx) : ORIGIN = 0x000000, LENGTH = 1M
	RAM (wx) : ORIGIN = 0x100000, LENGTH = 1M
}

SECTIONS
{
	TEXT : {
		*.o( CODE )
		*.a( CODE)
		. = ALIGN(4);
	}
	DATA: { 
		VBCC_CONSTRUCTORS
		*( __MERGED )
		*(.data DATA data)
		. = ALIGN(4);
	}
	BSS (NOLOAD): { 
		*(.bss BSS bss)
		*(COMMON)
	}
}
