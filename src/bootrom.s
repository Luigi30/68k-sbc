	include	"equates.s"

	global	_serial_char_out
	global	_serial_string_out
	global	_serial_in
	global	_serial_char_waiting
	
	global	_DATA_START
	global	_DATA_END
	global	_BSS_START
	global	_BSS_END

	global	_MEMTOP
	global	_MEMBOT
	
	xref	_Monitor_Go
	
	text

	org	$1000

	;; Main program
START:
	JSR	MFPINIT

	JSR	Copy_DataBSS
	
	JSR	_Monitor_Go
	
;;; ;;;;;;;;;;;;;;;;;;;;;;;;;;
Copy_DataBSS:
	move.l	#_DATA_END,d0
	move.l	#_DATA_START,d1
	sub.l	d1,d0
	
	move.l	#_DATA_START,a0			; start of .data
	move.l	#RAMSTART,a1
.copyLoop:
	move.b	(a0)+,(a1)+
	sub.l	#1,d0
	cmp.l	#0,d0
	bne		.copyLoop

	;; Okay, now clear the BSS section.
	move.l	#_DATA_END,d0	
	move.l	#_DATA_START,d1
	sub.l	d1,d0				; d0 = length of .data
	move.l	d0,d2
	
	move.l	#RAMSTART,a1
	add.l	d0,a1				; a1 = RAMSTART + .data length
	
	move.l	#_BSS_END,d0
	move.l	#_BSS_START,d1
	sub.l	d1,d0				; d0 = .bss length
	move.l	#0,d1
	add.l	d0,d2
	
.clearLoop:
	move.b	d1,(a1)+
	sub.l	#1,d0
	cmp.l	#0,d0
	bne		.clearLoop

	move.l	d2,_MEMBOT
	move.l	#$1FFFFF,_MEMTOP
	
	rts	
	
;;; ;;;;;;;;;;;;;;;;;;;;;;;;;;
MFPINIT:
	clr.l	d0
	subq	#1,d0		; d0 = all 1s
	move.b	d0,MFPDDR	; all pins are output

	move.b	#$11,MFPTCDCR	; divide by 4 in C/D CNTRL REG
	move.b	#%00001000,MFPUCR ; set up for 8-N-1
	
	;; Start the TX and RX clocks
	move.b	#1,MFPRSR
	move.b	#5,MFPTSR
	bset.b	#7,MFPGPIP 	; enable serial transmission

	rts

_serial_char_out:
	btst	#7,MFPTSR
	beq	_serial_char_out

	move.b	d0,MFPUDR
	rts
	
_serial_string_out:
	;; a0.L = message to print, null-terminated
	btst	#7,MFPTSR		; is there something in the transmit buffer?
	beq	_serial_string_out	; if yes, wait until it's empty

	move.b	(a0)+,d0
	move.b	d0,MFPUDR
	tst.b	d0			; null terminator?
	bne	_serial_string_out	; no, continue printing
	rts

_serial_in:
	;; return the character in d0
	clr.l	d0
	move.b	MFPUDR,d0
	rts
	
_serial_char_waiting:
	btst	#7,MFPRSR
	bne	.yes 		; bit 7 is set - character waiting
.no:
	move.l	#0,d0
	rts

.yes:
	move.l	#1,d0
	rts
	
;;; ;;;;;;;;;;;;;;;;;;;;;;;;;;;

	DATA

_MEMBOT dc.l 0
_MEMTOP dc.l 0
	
	end

