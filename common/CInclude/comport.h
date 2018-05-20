#ifndef COMPORT_H
#define COMPORT_H

/* ISA AT COM port card. */

#define ISA_IOBASE 0xFA0000
#define ISA_IO_ADDRESS(ADDR) ( ISA_IOBASE + (2 * (ADDR) ) )

#define COM1_DATA 0x3F8
#define COM1_INTEN 0x3F9
#define COM1_FIFO 0x3FA
#define COM1_LCR 0x3FB
#define COM1_MCR 0x3FC
#define COM1_LSR 0x3FD
#define COM1_MSR 0x3FE
#define COM1_SR 0x3FF

/* with DLAB == 1 */
#define COM1_BAUD_LSB 0x3F8
#define COM1_BAUD_MSB 0x3F9

#endif
