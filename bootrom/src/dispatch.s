* The dispatcher for all system calls.
* System calls are Line-A instructions.
* Since we get here through an exception, the stack contains:
*   CCR | PC.H | PC.L | (any function parameters)

	include "traptable.s"
	
	global  LineA_Dispatcher
	global	_DoLineATest

LineA_Dispatcher:
	;; preserve a2-a6/d2-d7 per VBCC ABI
	movem.l	a2-a7/d2-d7,-(sp)
	
* OK, now execute the system call.
    MOVE.L  (48+2,sp),a0		; Retrieve the emulated opcode
    MOVE.W  (a0),d0
    AND.L   #$5FFF,d0		    ; mask off the $A nybble

    ASL     #2,d0          		; multiply by 4 to get the jump table offset
    LEA     OSFunc_JumpTable,a0
    MOVE.L  (a0,d0.l),a1 		; a1 = jump table address

	move.l	(48+2,a7),d0		; grab the address of the Line-A instruction
	add.l	#2,d0				; add 2 to it, so we RTS to the next instruction
	move.l	d0,(48+2,a7)

    JSR     (a1)           		; JSR to the OS call.
	movem.l	(sp)+,a2-a7/d2-d7	; Restore any modified registers.
	RTE
	
_DoLineATest:
	dc.w	$A001
	rts
