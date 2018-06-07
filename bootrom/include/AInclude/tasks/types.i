    include "c_types.i"

 STRUCTURE  TT_Task,LN_SIZE    ; struct Task, derived from Node
    CPTR    TT_TaskInfo          ; TaskInfo *
    LABEL   TT_SIZE

 STRUCTURE  TI_TaskInfo,0   ; struct TaskInfo
    UBYTE   TI_State        ; TASK_State state
    UBYTE   TI_Heap         ; TASK_Heap heap
    
    CPTR    TI_StackLow     ; bottom of the alloc'ed stack
    CPTR    TI_StackHigh    ; top of the alloc'ed stack
    CPTR    TI_StackPointer ; task's SP in the saved context
    CPTR    TI_EntryPoint   ; task's entry point

    ; TODO: STRUCT SG_SIGBLOCK,0
    ULONG   TI_SigsRecv     ; Received signal flags
    ULONG   TI_SigsWait     ; Waiting on signal flags
    ; TODO: STRUCT MP_MSGPORT,0
    CPTR    TI_MsgPortName  ; Message port name
    CPTR    TI_MsgPortQueue ; List * message_queue

    UWORD   TI_StatusReg    ; saved context status register
    ULONG   TI_ProgramCntr  ; saved context PC
    ULONG   TI_SavedA0      ; saved context registers (16 ULONGs)
    ULONG   TI_SavedA1
    ULONG   TI_SavedA2
    ULONG   TI_SavedA3
    ULONG   TI_SavedA4
    ULONG   TI_SavedA5
    ULONG   TI_SavedA6
    ULONG   TI_SavedA7

    ULONG   TI_SavedD0
    ULONG   TI_SavedD1
    ULONG   TI_SavedD2
    ULONG   TI_SavedD3
    ULONG   TI_SavedD4
    ULONG   TI_SavedD5
    ULONG   TI_SavedD6
    ULONG   TI_SavedD7

    LABEL   TI_SIZE