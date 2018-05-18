  include "c_types.i"
  
  STRUCTURE FDPoint_Struct,0
	UWORD FDPoint_X
	UWORD FDPoint_Y
	LABEL FDPoint_SIZE
	
  STRUCTURE FDPen_Struct,0
	UWORD FDPen_PositionX
	UWORD FDPen_PositionY
	UWORD FDPen_SizeX
	UWORD FDPen_SizeY
	UBYTE FDPen_Color
	LABEL FDPen_SIZE
