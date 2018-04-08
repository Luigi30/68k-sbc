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
	}
			
	. = 0x100000;
	.data : { *(.data) }
	.bss : { *(.bss) }
}
