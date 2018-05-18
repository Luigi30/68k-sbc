PUSH MACRO
movem.l \1,-(sp)
  ENDM

POP MACRO
  movem.l (sp)+,\1
  ENDM
  
PUSHREGS MACRO
  movem.l d0-d7/a0-a6,-(sp)
  ENDM

POPREGS MACRO
  movem.l d0-d7/a0-a6,(sp)+
  ENDM
  
