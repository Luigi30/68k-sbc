ENTRY(_bios_start)

MEMORY
{
	ROM (rx) : ORIGIN = 0x000000, LENGTH = 1M
	RAM (wx) : ORIGIN = 0x100000, LENGTH = 1M
}

SECTIONS
{
	. = 0x0;
	.text : {
		crt0.o( CODE )
		. = 0x1000;
		bootrom.o( CODE )
		test.o( CODE )
		libc68k.a ( CODE)
	}
	.data : AT ( 0x100000 ) { 
		. = ALIGN( 0x1000 )
		_DATA_START = .;
		VBCC_CONSTRUCTORS
		*(.data DATA data) 
		_DATA_END = .;
	}
	.bss : AT ( 0x100000 + SIZEOF (.data) + 32 ) { 
		. =ALIGN( 0x1000 )
		_BSS_START = .;
		*(.bss BSS bss)
		*(COMMON)
		_BSS_END = .; 
	}
}
