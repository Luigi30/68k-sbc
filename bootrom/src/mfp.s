	public _MFP_ISR
	public _TIMERA_ISR
	public _RECV_BUFFER_ISR

	public _RECV_BUFFER_VECTOR
	
	public _ExecutingTaskRegisters
	public _NewTaskRegisters
	
	include	"equates.s"
	
	code
	
	even
_MFP_ISR:
	;; Back up the current registers.
	movem.l	a0-a7/d0-d7,-(sp)	
	
	;; Restore the registers.
	movem.l	(sp)+,a0-a7/d0-d7
	rts

_RECV_BUFFER_ISR:
	movem.l	a0-a7/d0-d7,-(sp)

	;; Acknowledge the interrupt
	move.b	MFPISRA,d0
	andi.b	#$EF,d0
	move.b	d0,MFPISRA

	;; Has anything hooked this interrupt?
	move.l	_RECV_BUFFER_VECTOR,a0
	cmp.l	#0,a0
	beq		.done 				; no, so we're done
	jsr		(a0)

.done:
	movem.l (sp)+,a0-a7/d0-d7
	rte
	
_TIMERA_ISR:
	;; Back up the current registers.
	move.l	a0,_ExecutingTaskRegisters+0
	move.l	a1,_ExecutingTaskRegisters+4
	move.l	a2,_ExecutingTaskRegisters+8
	move.l	a3,_ExecutingTaskRegisters+12
	move.l	a4,_ExecutingTaskRegisters+16
	move.l	a5,_ExecutingTaskRegisters+20
	move.l	a6,_ExecutingTaskRegisters+24
	move.l	a7,_ExecutingTaskRegisters+28

	move.l	d0,_ExecutingTaskRegisters+32
	move.l	d1,_ExecutingTaskRegisters+36
	move.l	d2,_ExecutingTaskRegisters+40
	move.l	d3,_ExecutingTaskRegisters+44
	move.l	d4,_ExecutingTaskRegisters+48
	move.l	d5,_ExecutingTaskRegisters+52
	move.l	d6,_ExecutingTaskRegisters+56
	move.l	d7,_ExecutingTaskRegisters+60

	move.w 	(sp),_SFRAME_SR
	move.l	2(sp),_SFRAME_PC

	;; Acknowledge the timer interrupt.
	move.b	MFPISRA,d0
	andi.b	#$DF,d0
	move.b	d0,MFPISRA

	jsr		_TASK_ProcessQuantum
	
	;; Restore the registers.
	move.l	_ExecutingTaskRegisters+0,a0
	move.l	_ExecutingTaskRegisters+4,a1
	move.l	_ExecutingTaskRegisters+8,a2
	move.l	_ExecutingTaskRegisters+12,a3
	move.l	_ExecutingTaskRegisters+16,a4
	move.l	_ExecutingTaskRegisters+20,a5
	move.l	_ExecutingTaskRegisters+24,a6
	move.l	_ExecutingTaskRegisters+28,a7

	move.l	_ExecutingTaskRegisters+32,d0
	move.l	_ExecutingTaskRegisters+36,d1
	move.l	_ExecutingTaskRegisters+40,d2
	move.l	_ExecutingTaskRegisters+44,d3
	move.l	_ExecutingTaskRegisters+48,d4
	move.l	_ExecutingTaskRegisters+52,d5
	move.l	_ExecutingTaskRegisters+56,d6
	move.l	_ExecutingTaskRegisters+60,d7

	;; Build a new exception frame.
	move.w	_SFRAME_SR,(sp)
	move.l	_SFRAME_PC,2(sp)
	rte

	data
_RECV_BUFFER_VECTOR dc.l 0
