* The dispatcher for all system calls.
* System calls are Line-A instructions.
* Since we get here through an exception, the stack contains:
*   CCR | PC.H | PC.L | (any function parameters)

	include "traptable.s"
	
	global  LineA_Dispatcher
	global	_DoLineATest

LineA_Dispatcher:
	;; preserve a2-a6/d2-d7 per VBCC ABI
	move.l	sp,usp				; back up SP
	move.l	#OldReturnAddr,a1
	move.l	2(a7),(a1)			; the original return address will get clobbered, so preserve it
	
	move.l	#RegPreserve,a1		
	movem.l	a2-a6/d2-d7,-(a1)	; and preserve a2-a6/d2-d7 per VBCC ABI
	
* OK, now execute the system call.
    MOVE.L  (2,sp),a0			; Retrieve the emulated opcode
    MOVE.W  (a0),d0
    AND.L   #$5FFF,d0		    ; mask off the $A nybble

    ASL     #2,d0          		; multiply by 4 to get the jump table offset
    LEA     OSFunc_JumpTable,a0
    MOVE.L  (a0,d0.l),a1 		; a1 = jump table address

	move.l	(2,a7),d0			; grab the address of the Line-A instruction
	add.l	#2,d0				; add 2 to it, so we RTS to the next instruction
	move.l	d0,(2,a7)

	add.l	#6,a7
	JSR     (a1)           		; JSR to the OS call.

	move.l	#RegPreserve,a1
	movem.l	(a1)+,a2-a6/d2-d7	; Restore any modified registers.

LineA_Return:
	move.l	usp,sp
	addq.l	#2,sp
	move.l	sp,usp
	
	move.l	#OldReturnAddr,a1	; grab the return address we saved
	move.l	(a1),2(sp)
	add.l	#2,2(sp)			; add 2 to it so we return to the instr after the Line-A
	RTE

	DATA
RegPreserve:	ds.l	16
OldReturnAddr:	ds.l	1
	
_DoLineATest:
	dc.w	$A001
	rts
