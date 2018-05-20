	public 	_MOUSE_PacketISR

	public 	_MOUSE_IncomingPacket
	public 	_MOUSE_PacketBytesReceived
	
	code

_MOUSE_PacketISR:
	movem.l	d0-d7/a0-a6,-(sp)

	move.l	#$FA0000,a0
	move.l	#$3F8,d0
	asl.l	#1,d0
	add.l	d0,a0

	move.w	(a0),d0

	;; Inspect the byte.
	;; If b6 is 1, this is the first byte of a packet.
	;; If b6 is 0, this is another byte of a packet.
	btst	#6,d0
	beq		.isExpectingPacketStart

.startNewPacket:
	move.b	#0,_MOUSE_PacketBytesReceived
	bra		.continuePacket
	
.isExpectingPacketStart:
	;; This is not a new packet. Are we expecting the start of a packet?
	cmp.b	#0,_MOUSE_PacketBytesReceived
	beq		_MOUSE_PacketProcessed				; Yes, so reject this byte.

	;; This is not a new packet and we aren't expecting one anyway.
.continuePacket:	
	move.l	#_MOUSE_IncomingPacket,a1
	move.b	_MOUSE_PacketBytesReceived,d1
	
	move.b	d0,0(a1,d1)
	addq.b	#1,_MOUSE_PacketBytesReceived

	cmpi.b	#3,_MOUSE_PacketBytesReceived
	beq		_MOUSE_ProcessPacket
	bra		_MOUSE_PacketProcessed

_MOUSE_ProcessPacket:
	jsr		_MOUSE_PacketToStruct
	
	move.b	#0,_MOUSE_PacketBytesReceived
	bra		_MOUSE_PacketProcessed
	
_MOUSE_PacketProcessed:	
	movem.l	(sp)+,d0-d7/a0-a6
	rts
	
	;; 
	data
_MOUSE_IncomingPacket		ds.b 3 ; 3 byte buffer for an incoming packet
_MOUSE_PacketBytesReceived 	dc.b 0 ; offset to put the next byte into the buffer

	
