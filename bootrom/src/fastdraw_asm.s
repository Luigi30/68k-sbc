	public _DRAW_PutPixel
	public _DRAW_PutFontGlyph
	
	code

DRAWPORT_vram_base = 0
DRAWPORT_size_x = 4
DRAWPORT_size_y = 6
DRAWPORT_depth = 8
DRAWPORT_SIZE = 10

_DRAW_PutPixel:
	;; d0.w = X
	;; d1.w = Y
	ext.l	d0
	ext.l	d1
	
	move.l	_currentPort,a0
	move.l	DRAWPORT_vram_base(a0),a0
	
	move.l	_currentPort+DRAWPORT_size_x,d2
	mulu	d1,d2
	add.l	d0,d2
	add.l	d2,a0
	;; a0 is now the pixel address
	
	move.b	_pen_fore,(a0)

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
