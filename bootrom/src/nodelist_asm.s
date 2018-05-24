*
*   List Node Structure.  Each member in a list starts with a Node
*
*    STRUCTURE	LN,0	; List Node
*	APTR	LN_SUCC	; Pointer to next (successor)
*	APTR	LN_PRED	; Pointer to previous (predecessor)
*	UBYTE	LN_TYPE
*	BYTE	LN_PRI	; Priority, for sorting
*	APTR	LN_NAME	; ID string, null terminated
*	LABEL	LN_SIZE	; Note: word aligned
	
*LN_SUCC	equ 0
*LN_PRED	equ 4
*LN_TYPE	equ 8
*LN_PRI	equ 9
*LN_NAME equ 10
*LN_SIZE	equ 26

; minimal node -- no type checking possible
*   STRUCTURE	MLN,0	; Minimal List Node
*	APTR	MLN_SUCC
*	APTR	MLN_PRED
*	LABEL	MLN_SIZE

*MLN_SUCC	equ 0
*MLN_PRED	equ 4
*MLN_TYPE	equ 8

*STRUCTURE	LH,0
*	APTR	LH_HEAD
*	APTR	LH_TAIL
*	APTR	LH_TAILPRED
*	UBYTE	LH_TYPE
*	UBYTE	LH_pad
*	LABEL	LH_SIZE ;word aligned

*LH_HEAD		equ 0
*LH_TAIL		equ 4
*LH_TAILPRED equ 8
*LH_TYPE		equ 12
*LH_pad		equ 13
*LH_SIZE		equ 14

*
* Minimal List Header - no type checking (best for most applications)
*
*STRUCTURE	MLH,0
*	APTR	MLH_HEAD
*	APTR	MLH_TAIL
*	APTR	MLH_TAILPRED
*	LABEL	MLH_SIZE ;longword aligned

*MLH_HEAD	 equ 0
*MLH_TAIL	 equ 4
*MLH_TAILPRED equ 8
*MLH_SIZE	 equ 12

	include 'c_types.i'

***********************************************

	STRUCTURE	LN,0		; List Node
	APTR	LN_SUCC			; Pointer to next (successor)
	APTR	LN_PRED			; Pointer to previous (predecessor)
	UBYTE	LN_TYPE
	BYTE	LN_PRI				; Priority, for sorting
	APTR	LN_NAME				; ID string, null terminated
	LABEL	LN_SIZE				; Note: word aligned

	;;  minimal node -- no type checking possible
	STRUCTURE	MLN,0		; Minimal List Node
	APTR	MLN_SUCC
	APTR	MLN_PRED
	LABEL	MLN_SIZE

	*
	* Full featured list header
	*
	STRUCTURE	LH,0
	APTR	LH_HEAD
	APTR	LH_TAIL
	APTR	LH_TAILPRED
	UBYTE	LH_TYPE
	UBYTE	LH_pad
	LABEL	LH_SIZE 			;word aligned

	*
	* Minimal List Header - no type checking (best for most applications)
	*
	STRUCTURE	MLH,0
	APTR	MLH_HEAD
	APTR	MLH_TAIL
	APTR	MLH_TAILPRED
	LABEL	MLH_SIZE 			;longword aligned

***********************************************

	xdef	_LIST_AddHead
	xdef	_LIST_AddTail
	xdef	_LIST_RemHead
	xdef	_LIST_RemTail
	xdef	_LIST_Remove

	even
_LIST_AddHead:
	MOVE.L  (A0),D0			;d0 = succ(a0)
	MOVE.L  A1,(A0)			;new node = (a0)
	MOVEM.L D0/A0,(A1)		
	MOVE.L  D0,A0
	MOVE.L  A1,LN_PRED(A0)	;new node = predecessor of the old head
	RTS

	even
_LIST_AddTail:
	ADDQ.L  #LH_TAIL,A0
	MOVE.L  LN_PRED(A0),D0
	MOVE.L  A1,LN_PRED(A0)
	MOVE.L  A0,(A1)
	MOVE.L  D0,LN_PRED(A1)
	MOVE.L  D0,A0
	MOVE.L  A1,(A0)
	RTS

	even
_LIST_RemHead:
	MOVE.L  (A0),A1
	MOVE.L  (A1),D0
	BEQ.S   .done
	MOVE.L  D0,(A0)
	EXG.L   D0,A1
	MOVE.L  A0,LN_PRED(A1)
.done:
	RTS

	even
_LIST_RemTail:
	MOVE.L  LH_TAIL+LN_PRED(A0),A1
	MOVE.L  LN_PRED(A1),D0
	BEQ.S   .done
	MOVE.L  D0,LH_TAIL+LN_PRED(A0)
	EXG.L   D0,A1
	MOVE.L  A0,(A1)
	ADDQ.L  #4,(A1)
.done:
	RTS

	even
_LIST_Remove:
	MOVE.L  (A1),A0
	MOVE.L  LN_PRED(A1),A1
	MOVE.L  A0,(A1)
	MOVE.L  A1,LN_PRED(A0)
	RTS
