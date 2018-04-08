	include	"equates.s"

	text

	org	$1000

	;; Main program
START:
	JSR	MFPINIT
	move.l	#BootBanner,a0
	jsr	_serial_string_out

InitMonitorPrompt:
	JSR	Monitor_ClearPromptBuffer
	move.l	#PromptInputBuffer,a5

	move.l	#CommandPrompt,a0
	jsr	_serial_string_out

MonitorLoop:
	jsr	_serial_char_waiting
	tst.b	d0
	beq	MonitorLoopDone	; nothing waiting

	jsr	_serial_in	; get the character

	cmp.b	#13,d0
	beq	Monitor_GotCR
	move.b	d0,(a5)+	; save the character
	jsr	_serial_char_out ; print the character

MonitorLoopDone:	
	bra	MonitorLoop

Monitor_GotCR:
	JSR	Monitor_ProcessCommand
	bra	InitMonitorPrompt

	
;;; ;;;;;;;;;;;;;;;;;;;;;;;;;;
MFPINIT:
	clr.l	d0
	subq	#1,d0		; d0 = all 1s
	move.b	d0,MFPDDR	; all pins are output
*	move.b	#3,MFPTCDR 	; 1/4 Tx clock
*	move.b	#3,MFPTDDR 	; 1/4 Rx clock
*	move.b	#$11,MFPTCDCR	; divide by 4 in C/D CNTRL REG
*	move.b	#%10001000,MFPUCR ; set up for 8-N-1, clock/16

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
	
BootBanner:	dc.b "Luigi SBC ROM Monitor - 2018-04-06",10,13,10,13,0
HelloMsg:	dc.b "Hello, dumb terminal!",10,13,0

Monitor_BadCommand:	dc.b "Invalid command",10,13,0
	
;;; ;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Monitor functions
	
	even
Monitor_ClearPromptBuffer:
	;; Fill the prompt buffer with NUL.
	move.l	#PromptInputBuffer,a5
	move.b	#0,d0
	move.w	#255,d1

.clearLoop:
	move.b	d0,(a5)+
	dbra	d1,.clearLoop
	
	rts

	even
Monitor_ProcessCommand:
	move.l	#CRLF,a0
	jsr	_serial_string_out

	;; Check for a valid command...

	;; If not valid, fall through and return.
	move.l	#Monitor_BadCommand,a0
	jsr	_serial_string_out
	
	move.l	#CRLF,a0
	jsr	_serial_string_out
	rts
	
;;; Monitor strings
CRLF:		dc.b	10,13,0
CommandPrompt:	dc.b "> ",0

;;; ;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Monitor variables
INPUT_BUFFER_SIZE equ 256
	
PromptInputBuffer:	ds.b	INPUT_BUFFER_SIZE
	end
