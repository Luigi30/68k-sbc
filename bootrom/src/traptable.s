OSFunc_JumpTable:
	dc.l	$1			; A000, force a bus error
	dc.l	_OSFunc_Test			; A001
	dc.l	_simple_printf			; A002 | a0 = char * | a1 = ??? <- variadic todo
	dc.l	_PrintStringToDevice 	; A003 | a0 = char * | a1 = ??? <- variadic todo
