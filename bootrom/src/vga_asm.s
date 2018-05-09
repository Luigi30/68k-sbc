	public _VGA_SetPixel
	public _VGA_SetMode12Pixel

	code

VGA_MODE_80x25 = $03
VGA_MODE_12h = $12
VGA_MODE_13h = $13
VGA_MODE_2Eh = $2E

	even
_VGA_SetPixel:
	move.l	_VGA_MODE,d3
	cmp		#VGA_MODE_12h,d3
	beq		_VGA_SetMode12Pixel
	cmp		#VGA_MODE_13h,d3
	beq		_VGA_SetMode13Pixel
	cmp		#VGA_MODE_2Eh,d3
	beq		_VGA_SetMode2EPixel
	rts

_VGA_SetMode12Pixel:
	;; 640x480 planar (4bpp)
	;; d0.w = x
	;; d1.w = y
	;; d2.b = color

	move.l	#$F00000,a0
	move.b	d0,(a0)

	;; load the X and Y coords for calculation of offset
	move.w	d1,d3
	ext.l	d3
	move.w	d1,d4
	ext.l	d4
	move.w	d0,d5
	ext.l	d5

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

_VGA_SetMode13Pixel:
	;; 320x200 linear (8bpp)
	;; d0.w = x
	;; d1.w = y
	;; d2.b = color
	move.w	#320,d7
	move.l	#$800001,a0

	ext.l	d0
	move.w	d1,d3
	ext.l	d3
	mulu.w	d7,d3
	add.l	d0,d3

	add.l	#$A0000,d3
	asl.l	#1,d3
	add.l	d3,a0

	move.b	d2,(a0)
	
	rts
	
_VGA_SetMode2EPixel:
	;; 640x480 linear (8bpp)
	;; d0.w = x
	;; d1.w = y
	;; d2.b = color

	;; Divide d1.w by 104 (64 + 32 + 8) to get the correct segment.
	move.w	#640,d7
	move.l	#$900001,a0

	ext.l	d0
	move.w	d1,d3
	ext.l	d3
	mulu.w	d7,d3
	add.l	d0,d3
	add.l	d3,a0

	move.b	d2,(a0)
	
	rts
