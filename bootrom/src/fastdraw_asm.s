	public _DRAW_PutPixel
	public _DRAW_PutFontGlyph
	public _DRAW_LineTo
	public _DRAW_ScreenFill
	
	include "stack.i"
	include	"c_types.i"
	include "fastdraw/types.i"
	
	code

DRAWPORT_vram_base = 0
DRAWPORT_size_x = 4
DRAWPORT_size_y = 6
DRAWPORT_depth = 8
DRAWPORT_SIZE = 10

_DRAW_PutPixel:
	;; d0.l = X
	;; d1.l = Y
	move.l	4(sp),d0
	move.l	8(sp),d1

	move.l	d2,-(sp)

	move.l	_currentPort,a0
	move.w	DRAWPORT_size_x(a0),d2
	move.l	DRAWPORT_vram_base(a0),a0
	
	mulu	d1,d2
	add.l	d0,d2
	add.l	d2,a0
	;; a0 is now the pixel address
	move.b	_pen_fore,(a0)

	move.l	(sp)+,d2
	rts

_DRAW_PutFontGlyph:
	;; A font bitmap is 256x256.
	;; a0.l = address of font pixels
	;; d0.w = X coordinate to put glyph
	;; d1.w = Y coordinate to put glyph
	;; d2.b = ASCII codepoint

	move.l	_currentPort,a1
	move.l	DRAWPORT_vram_base(a1),a1 
	;; a1 = DrawPort's pixel buffer
	
	;; Now calculate the write address.
	move.l	_currentPort,a2
	move.l	DRAWPORT_size_x(a2),d3
	mulu	d1,d3
	add.l	d0,d3
	add.l	d3,a1 				; a1 is now the write destination

	;; Now calculate the read address.
	move.b	d2,d6
	ext.w	d6
	move.b	d2,d7
	ext.w	d7

	andi.w	#$0F,d6				; X coordinate
	asl.w	#4,d6
	andi.b	#$F0,d7				; Y coordinate

	mulu.w	#256,d7
	add.w	d6,d7
	add.l	d7,a0

	;; Copy a 16x16 block from (a0) to (a1).
	move.w	#15,d7
	move.w	#15,d6

	move.l	#256-16,d0			; source stride
	move.l	#640-16,d1			; destination stride

RowLoop:

ColumnLoop:
	move.b	(a0)+,(a1)+
	dbra.w	d7,ColumnLoop

	add.l	d0,a0
	add.l	d1,a1
	move.w	#15,d7
	dbra.w	d6,RowLoop
	
	rts

_DRAW_LineTo:
	;; d0.w = x1
	;; d1.w = y1
	movem.l	a2-a6/d2-d7,-(sp)

	LINK	a6,#-8
	move.w	d0,a1			; a1 = x2
	move.w	d1,a2			; a2 = y2
	
	move.l	#_penLocation,a5
	move.w	FDPoint_X(a5),d2	; d2 = penLocation.X
	move.w	FDPoint_Y(a5),d3	; d3 = penLocation.Y

	move.w	d0,d4
	move.w	d1,d5
	sub.w	d2,d4 				; d4 = deltaX
	sub.w	d3,d5				; d5 = deltaY

	;; calculate the signs of deltaX and deltaY
.checkDeltaX:
	tst.w	d4
	bpl.w	.checkDeltaY
	neg.w	d4
	ext.l	d4

.checkDeltaY:
	tst.w	d5
	bmi.w	.checkAddX
	neg.w	d5
	ext.l	d5

	;; calculate addX and addY
.checkAddX:
	cmp.w	d2,d0
	bhi.w	.xIsPositive
.xIsNegative:
	move.w	#-1,d2
	bra		.checkAddY
.xIsPositive:
	move.w	#1,d2

.checkAddY:
	cmp.w	d3,d1
	ble.w	.yIsNegative
.yIsPositive:
	move.w	#1,d3
	bra		.calcError
.yIsNegative:
	move.w	#-1,d3

	;; error = d7
	;; error2 = d6
.calcError:
	move.w	d4,d7
	add.w	d5,d7
	
	;; the while loop
	;; this is fucked up...
.drawLinePixel:
	move.w	FDPoint_X(a5),d0
	move.w	FDPoint_Y(a5),d1
	
	move.l	d1,-(sp)
	move.l	d0,-(sp)
	jsr		_DRAW_PutPixel
	move.l	(sp)+,d0
	move.l	(sp)+,d1

.checkX:						
	cmp.w	d0,a1				; x1 == x2?
	bne		.nextPixel
.checkY:
	cmp.w	d1,a2				; y1 == y2?
	beq		.done

.nextPixel:
	move.w	d7,d6
	asl.w	#1,d6				; d6 = error2

.checkErrorY:
	;; if error2 >= deltaY
	cmp.w	d5,d6
	blt		.checkErrorX
	add.w	d5,d7				; add deltaY to error
	add.w	d2,FDPoint_X(a5)	; add addX to penLocation.x
	
.checkErrorX:
	;; if error2 <= deltaX
	cmp.w	d6,d4
	blt		.doLoop
	add.w	d4,d7				; add deltaX to error
	add.w	d3,FDPoint_Y(a5)	; add addY to penLocation.y

.doLoop:
	bra		.drawLinePixel
	
.done:
	UNLK	a6

	movem.l	(sp)+,a2-a6/d2-d7
	rts

;;; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_DRAW_ScreenFill:
	;; d0.b - color index

	move.l	_currentPort,a0
	move.l	DRAWPORT_vram_base(a0),a1
	move.w	DRAWPORT_size_x(a0),d7
	move.w	DRAWPORT_size_y(a0),d6
	
.rowLoop:
	move.w	DRAWPORT_size_x(a0),d7
	lsr		#2,d7
	
.columnLoop:
	move.l	#$07070707,(a1)+
	dbra	d7,.columnLoop

	dbra	d6,.rowLoop
	
	rts
