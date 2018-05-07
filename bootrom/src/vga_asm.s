	public _VGA_SetPixel
	public _VGA_SetMode12Pixel

	code

VGA_MODE_80x25 = $03
VGA_MODE_12h = $12
VGA_MODE_13h = $13

	even
_VGA_SetPixel:
	move.l	_VGA_MODE,d3
	cmp		#VGA_MODE_12h,d3
	beq		_VGA_SetMode12Pixel
	cmp		#VGA_MODE_13h,d3
	beq		_VGA_SetMode13Pixel
	rts

_VGA_SetMode12Pixel:
	;; d0.w = x
	;; d1.w = y
	;; d2.b = color

	;; update the bitmask
	move.l	#$FA079C,a0
	move.w	#$08,(a0)+
	move.w	d0,d3
	andi.b	#$07,d3
	move.w	#$80,d4
	lsr		d3,d4
	move.w	d4,(a0)

	;; load the X and Y coords for calculation of offset
	move.w	d1,d3
	ext.l	d3
	move.w	d1,d4
	ext.l	d4
	move.w	d0,d5
	ext.w	d5

	;; offset = ((d3 << 6) + (d4 << 4)) + (d5 >> 3)
	asl.w	#6,d3
	asl.w	#4,d4
	add.w	d4,d3

	asr.w	#3,d5
	add.w	d5,d3

	;; a0 = 0x800000 + (2 * (0xA0000 + offset))
	move.l	#$800001,a0
	
	add.l	#$A0000,d3
	asl.l	#1,d3
	add.l	d3,a0

	;; read to set the latches, then write to update VRAM
	move.b	(a0),d4
	move.b	d2,(a0)

	rts
	
