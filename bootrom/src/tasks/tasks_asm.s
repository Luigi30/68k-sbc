	include "equates.s"
	include "c_types.i"
	include "stack.i"
	include "list.i"

	include "stdio.i"

	; methods
	public	_TASK_InitSubsystem
	public	_TASK_ProcessQuantumASM
	public	_TASK_SwitchingEnabled

	; vars
	public	_TASK_RunningTask

	code

H_SYSHEAP equ 1

_TASK_ProcessQuantumASM:
	; The current timeslice has expired.

	; Is the scheduler enabled?
	tst.b	_TASK_SwitchingEnabled
	beq		TASK_ExitScheduler

	; The scheduler is enabled. Perform a context switch if required.
	JSR		_TASK_FindReadyTaskASM ; a0 = the next task to execute
	JSR		_TASK_ContextSwitchASM

	PUSH	#str_TODO
	JSR		_simple_printf
	FIXSTAK #8
.loop:
	bra		.loop

	bra		TASK_ExitScheduler

***********************
* Perform a context switch to the task provided in a0.
*
_TASK_ContextSwitchASM:
	PUSHREGS
	LINK	a6,#-16
	move.l	a0,-4(a6) 	; -4(a6) = new_task

; Is a context switch required?
	move.l	-4(a6),a0
	cmp.l	_TASK_RunningTask,a0
	bne		.performContextSwitch

	; No switch is required if running task == new task
	bra		.done

.performContextSwitch:
	PUSH	a0
	PUSH	#str_ContextSwitch
	JSR		_simple_printf
	FIXSTAK #8

.done:
	UNLK	a6
	POPREGS
	RTS

***********************
* Find the next task to execute.
* - Pop a task off LIST_ReadyList.
* - If LIST_ReadyList is empty, return the currently running task.
* Returns a task pointer in a0.
_TASK_FindReadyTaskASM:
	PUSHR a2-a6/d2-d6

; Is LIST_ReadyList empty?
	move.l	_TASK_ReadyList,a0
	CMP.L   LH_TAIL+LN_PRED(a0),a0
	BEQ     .returnRunningTask
; It's not empty so return the head node in a0.
	REMHEAD
	move.l	d0,a0
	PUSH	a0
	PUSH	#str_TaskIsReady
	JSR		_simple_printf
	FIXSTAK	#4			; pop string address, keep a0
	move.l	(sp)+,a0
	bra		.done

.returnRunningTask:
	move.l	_TASK_RunningTask,a0
	PUSH	a0
	PUSH	#str_NoTaskIsReady
	JSR		_simple_printf
	FIXSTAK #4
	move.l	(sp)+,a0

.done:
	POPR	a2-a6/d2-d6
	RTS

***********************
TASK_ExitScheduler:
	RTS

***********************
_TASK_InitSubsystem:
	PUSHREGS

	; Initialize the two task lists
	PUSH	#H_SYSHEAP
	PUSH	#LH_SIZE
	JSR		_MEMMGR_NewPtr
	FIXSTAK	#8
	move.l	d0,_TASK_WaitingList
	move.l	d0,a0
	NEWLIST	a0

	PUSH	#H_SYSHEAP
	PUSH	#LH_SIZE
	JSR		_MEMMGR_NewPtr
	FIXSTAK	#8
	move.l	d0,_TASK_ReadyList
	move.l	d0,a0
	NEWLIST	a0

	; Set up timers and interrupt vectors
	move.b	#$40,MFPVR
	
	move.b	MFPIERA,d0
	or.b	#$20,d0
	move.b	d0,MFPIERA

	move.b	#$17,MFPTACR

	; Initialize task variables
	clr.l	_TASK_RunningTask

	POPREGS
	RTS

* Messages
str_NoTaskIsReady: 	dc.b "No task is ready. Returning the running task: %06X",NL,0
str_TaskIsReady:	dc.b "Task %06X is ready",NL,0

str_ContextSwitch:	dc.b "*** Context Switch to Task %06X ***",NL,0

str_TODO:			dc.b "TODO: the rest of the task scheduler",NL,0

	data
	cnop 0,2
_TASK_SwitchingEnabled 	dc.b 0
_TASK_RunningTask		dc.l 0