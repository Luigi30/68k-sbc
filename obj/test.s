	idnt	"src/test.c"
	opt	0
	opt	NQLPSMRBT
	section	"CODE",code
	public	_tester
	cnop	0,4
_tester
	subq.w	#4,a7
	movem.l	l3,-(a7)
	move.l	#0,(0+l5,a7)
	move.l	(0+l5,a7),d0
l1
l3	reg
l5	equ	0
	addq.w	#4,a7
	rts
; stacksize=4
