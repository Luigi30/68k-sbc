	idnt	"test.c"
	opt	0
	opt	NQLPSMRBT
	section	"CODE",code
	public	_main
	cnop	0,4
_main
	movem.l	l5,-(a7)
	addq.l	#2,_x
	subq.l	#2,_y
	lea	l3,a0
	move.l a0,-(a7)
	dc.w $A002
	add.l #8,sp

	move.l	#0,a1
	lea	l4,a0
	moveq	#0,d0
	move.l a1,-(a7)
 	move.l a0,-(a7)
 	move.l d0,-(a7)
 	dc.w $A003
 	add.l #16,sp

	moveq	#0,d0
l1
l5	reg
l7	equ	0
	rts
	cnop	0,4
l3
	dc.b	104
	dc.b	101
	dc.b	114
	dc.b	101
	dc.b	32
	dc.b	105
	dc.b	115
	dc.b	32
	dc.b	97
	dc.b	32
	dc.b	115
	dc.b	116
	dc.b	114
	dc.b	105
	dc.b	110
	dc.b	103
	dc.b	32
	dc.b	112
	dc.b	97
	dc.b	115
	dc.b	115
	dc.b	101
	dc.b	100
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	116
	dc.b	111
	dc.b	32
	dc.b	79
	dc.b	83
	dc.b	70
	dc.b	117
	dc.b	110
	dc.b	99
	dc.b	32
	dc.b	36
	dc.b	65
	dc.b	48
	dc.b	48
	dc.b	50
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	32
	dc.b	65
	dc.b	48
	dc.b	46
	dc.b	76
	dc.b	10
	dc.b	0
	cnop	0,4
l4
	dc.b	116
	dc.b	101
	dc.b	115
	dc.b	116
	dc.b	10
	dc.b	0
	public	_x
	section	"DATA",data
	cnop	0,4
_x
	dc.l	0
	public	_y
	cnop	0,4
_y
	dc.l	0
