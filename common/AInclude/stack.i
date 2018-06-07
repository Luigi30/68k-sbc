PUSH MACRO
  move.l \1,-(sp)
  ENDM

POP MACRO
  move.l (sp)+,\1
  ENDM

PUSHR MACRO
  movem.l \1,-(sp)
  ENDM

POPR  MACRO
  movem.l (sp)+,\1
  ENDM
  
PUSHREGS MACRO
  movem.l d0-d7/a0-a6,-(sp)
  ENDM

POPREGS MACRO
  movem.l (sp)+,d0-d7/a0-a6
  ENDM
  
FIXSTAK MACRO
  addq  \1,sp
  ENDM
