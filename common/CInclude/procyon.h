#ifndef PROCYON_H__
#define PROCYON_H__

#include "mmio.h"
#include "types.h"

/* Global variables.
 * See sysvars.s for definitions. */
extern long RAMBase;
extern long RAMEnd;

extern long SysHeap;
extern long SysHeapEnd;

extern long ApplHeap;
extern long ApplHeapEnd

extern long ROMBase;
extern long ROMEnd;

extern long ROMDiskBase;

//#define P_MEMTOP 0x100000
//#define P_MEMBOT 0x100004

//#define P_HEAPBOT  0x100008
//#define P_HEAPTOP  0x10000C

/* Constants, memory addresses for hardware and such. */
//#define P_ROMSTART 0x000000
//#define P_ROMEND   0x0FFFFF

//#define P_RAMSTART 0x100000
//#define P_RAMEND   0x1FFFFF

//#define P_ROMDISK  0x80000

#endif
