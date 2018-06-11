	include "equates.s"
	include "c_types.i"
	include "stack.i"
	include "list.i"

	include "tasks/types.i"

	include "stdio.i"

	; methods
	public	_TASK_InitSubsystem
	public	_TASK_ProcessQuantumASM
	public	_TASK_SwitchingEnabled
	public _TASK_AllowInterrupts
	public _TASK_ForbidInterrupts

	; vars
	public	_TASK_RunningTask
	public	_TASK_SFRAME_PC
	public	_TASK_SFRAME_SR

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

*	PUSH	_TASK_ReadyList
*	JSR		_TASK_PrintTaskList
*	FIXSTAK #4

	JSR		_TASK_AllowInterrupts
	bra		TASK_ExitScheduler

***********************
* Enable the timer interrupt so we process timeslices.
_TASK_AllowInterrupts:
	or.b	#$20,MFPIMRA
	RTS

* Disable the timer interrupt so we don't process timeslices.
_TASK_ForbidInterrupts:
	and.b	#$DF,MFPIMRA
	RTS

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
*	PUSH	a0
*	PUSH	#str_ContextSwitch
*	JSR		_simple_printf
*	FIXSTAK #8

.curTaskNull:
	move.l	_TASK_RunningTask,a0
	cmp.l	#0,a0
	bne		.backupOldContext

* Current task is NULL, so nothing to back up.
*	PUSH	#str_CurTaskNull
*	JSR		_simple_printf
*	FIXSTAK	#4
	bra		.setupNewTask

.backupOldContext:
* Back up the current task context.
	JSR		_TASK_BackupOldContext

* Enqueue the old task.
	move.l	_TASK_ReadyList,a0
	move.l	_TASK_RunningTask,a1
	ADDTAIL

* Fallen out of the if/else block.
.setupNewTask:
	JSR		TASK_RestoreSavedContext

	move.l	-4(a6),a0
	move.l	TT_TaskInfo(a0),a1
	add.l	#TI_State,a1
	move.l	#TS_RUNNING,(a1)

	move.l	-4(a6),_TASK_RunningTask

.done:
	UNLK	a6
	POPREGS
	RTS

***********************
TASK_RestoreSavedContext:
*	pea		str_RestoringCtx
*	JSR		_simple_printf
*	FIXSTAK	#4

	move.w	#15,d0
	move.l	#_ExecutingTaskRegisters,a0
	move.l	-4(a6),a1	; base of new task
	move.l	TT_TaskInfo(a1),a1	; dereference, point to TaskInfo structure

	move.l	a1,a2
	add.l	#TI_SavedA0,a2

.loop:
	move.l	(a2)+,(a0)+	; copy saved registers to our array
	dbra	d0,.loop

	move.l	TI_StackPointer(a1),d0
	move.l	#_ExecutingTaskRegisters,a0
	add.l	#4*7,a0
	move.l	d0,(a0)

	move.l	TI_ProgramCntr(a1),_TASK_SFRAME_PC
	move.w	TI_StatusReg(a1),_TASK_SFRAME_SR

*	PUSH	-4(a6)
*	JSR		_TASK_Test
*	FIXSTAK	#4

*	pea		str_RestoreCtxDone
*	JSR		_simple_printf
*	FIXSTAK	#4

	RTS

***********************
*
_TASK_BackupOldContext:
*	PUSH	_TASK_RunningTask
*	PUSH	#str_BackingUpCtx
*	JSR		_simple_printf
*	FIXSTAK	#8

* All the crap inside the ELSE block of TASK_ContextSwitch
	move.l	_TASK_RunningTask,a0

	move.l	TT_TaskInfo(a0),a1
	add.l	#TI_SavedA0,a1

	move.w	#15,d0
	move.l	#_ExecutingTaskRegisters,a2

.saveRegsLoop:
	move.l	(a2)+,(a1)+
	dbra	d0,.saveRegsLoop

	move.l	#_ExecutingTaskRegisters+28,a2

	move.l	TT_TaskInfo(a0),a1
	add.l	#TI_StackPointer,a1
	move.l	(a2),(a1)

	move.l	TT_TaskInfo(a0),a1
	add.l	#TI_ProgramCntr,a1
	move.l	_TASK_SFRAME_PC,(a1)

	move.l	TT_TaskInfo(a0),a1
	add.l	#TI_StatusReg,a1
	move.l	_TASK_SFRAME_SR,(a1)

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
*	PUSH	a0
*	PUSH	#str_TaskIsReady
*	JSR		_simple_printf
*	FIXSTAK	#4			; pop string address, keep a0
*	move.l	(sp)+,a0
	bra		.done

.returnRunningTask:
	move.l	_TASK_RunningTask,a0
*	PUSH	a0
*	PUSH	#str_NoTaskIsReady
*	JSR		_simple_printf
*	FIXSTAK #4
*	move.l	(sp)+,a0

.done:
	POPR	a2-a6/d2-d6
	RTS

***********************
TASK_ExitScheduler:
*	pea		str_ExitScheduler
*	JSR		_simple_printf
*	FIXSTAK #4

	RTS

***********************
_TASK_WaitForMessage:
	JSR		_TASK_ForbidInterrupts
	; Is there a running task?

	cmp.l	#0,_TASK_RunningTask
	bne		.setTaskToWaiting

	pea		str_WaitingOnNull
	jsr		_simple_printf
	FIXSTAK	#4
	jsr		TASK_SysHalt

	; If there is a running task, mark it as waiting
	; for SIG_MESSAGE.
	; Add it to WaitingList and remove it from ReadyList.

.setTaskToWaiting:
	; TASK_RunningTask->info->state = TASK_WAITING;
    ; TASK_RunningTask->info->signals.waiting |= SIG_MESSAGE;

	move.l	_TASK_ReadyList,a0
	move.l	_TASK_RunningTask,a1
	REMOVE

	move.l	_TASK_WaitingList,a0
	move.l	_TASK_RunningTask,a1
	ADDTAIL

.done:
	rts

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
	clr.l	_TASK_SFRAME_PC
	clr.l	_TASK_SFRAME_SR

	POPREGS
	RTS

TASK_SysHalt:
	pea		str_SysHalt
	JSR		_simple_printf
.loop
	bra		.loop

* Messages
str_NoTaskIsReady 	dc.b "No task is ready. Returning the running task: %06X",NL,0
str_TaskIsReady		dc.b "Task %06X is ready",NL,0

str_ContextSwitch	dc.b "*** Context Switch to Task %06X ***",NL,0
str_CurTaskNull		dc.b "Current task is NULL. First run?",NL,0
str_BackingUpCtx	dc.b "Backing up context of task %06X",NL,0

str_RestoringCtx	dc.b "Restoring context of task",NL,0
str_RestoreCtxDone	dc.b "Context restored",NL,0

str_WaitingOnNull	dc.b "*** Task error! Waiting on a null task.",NL,0

str_TODO:			dc.b "TODO: the rest of the task scheduler",NL,0

str_ExitScheduler	dc.b "Exiting task scheduler...",NL,0

str_SysHalt			dc.b "System halted.",NL,0

	data
	cnop 0,2
_TASK_SwitchingEnabled 	dc.b 0
_TASK_RunningTask		dc.l 0
_TASK_SFRAME_SR			dc.w 0
_TASK_SFRAME_PC			dc.l 0