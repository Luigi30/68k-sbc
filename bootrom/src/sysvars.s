	data

	org	$100000

	MACRO	SysVar
_\1	ds.l	1
	global	_\1
	ENDM

SysVars:
	SysVar 	RAMBase		; $100004 - Bottom of RAM.
	SysVar	RAMEnd		; $100000 - Top of RAM.

	SysVar	SysHeap		; $100008 - Bottom of the system heap.
	SysVar	SysHeapEnd	; $10000C - Top of the system heap.

	SysVar	ApplHeap	; $100010 - Bottom of the application heap.
	SysVar	ApplHeapEnd	; $100014 - Top of the application heap.

	SysVar	ROMBase		; $100018 - Bottom of ROM address space.
	SysVar	ROMEnd		; $10001C - Top of ROM address space.

	SysVar	ROMDiskBase	; $100020 - Base address of the ROM disk.
