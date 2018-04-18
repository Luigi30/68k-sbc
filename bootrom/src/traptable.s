OSFunc_JumpTable:
	dc.l	$1					; A000, force a bus error
	dc.l	_OSFunc_Test		; A001
