#ifndef DMA_H
#define DMA_H

#define DMA_BASE 0xF00000

#define DMA_CSR(CH)  ( DMA_BASE + (0x40 * CH) + 0x00 )
#define DMA_CER(CH)  ( DMA_BASE + (0x40 * CH) + 0x01 )
#define DMA_DCR(CH)  ( DMA_BASE + (0x40 * CH) + 0x04 )
#define DMA_OCR(CH)  ( DMA_BASE + (0x40 * CH) + 0x05 )
#define DMA_SCR(CH)  ( DMA_BASE + (0x40 * CH) + 0x06 )
#define DMA_CCR(CH)  ( DMA_BASE + (0x40 * CH) + 0x07 )
#define DMA_MTCR(CH) ( DMA_BASE + (0x40 * CH) + 0x0A )
#define DMA_MAR(CH)  ( DMA_BASE + (0x40 * CH) + 0x0C )
#define DMA_DAR(CH)  ( DMA_BASE + (0x40 * CH) + 0x14 )
#define DMA_BTCR(CH) ( DMA_BASE + (0x40 * CH) + 0x1A )
#define DMA_BAR(CH)  ( DMA_BASE + (0x40 * CH) + 0x1C )
#define DMA_NIVR(CH) ( DMA_BASE + (0x40 * CH) + 0x25 )
#define DMA_EIVR(CH) ( DMA_BASE + (0x40 * CH) + 0x27 )
#define DMA_MFCR(CH) ( DMA_BASE + (0x40 * CH) + 0x29 )
#define DMA_CPR(CH)  ( DMA_BASE + (0x40 * CH) + 0x2D )
#define DMA_DFCR(CH) ( DMA_BASE + (0x40 * CH) + 0x31 )
#define DMA_BFCR(CH) ( DMA_BASE + (0x40 * CH) + 0x39 )
#define DMA_GCR(CH)  ( DMA_BASE + (0x40 * CH) + 0x3F )

/* Flags */

/* CPR */
#define DMA_PRI0 0x00 // highest
#define DMA_PRI1 0x01
#define DMA_PRI2 0x02
#define DMA_PRI3 0x03 // lowest

/* DCR */
#define DMA_XRM_BURST 0x00
#define DMA_XRM_STEAL 0x40
#define DMA_XRM_STEAL_HOLD 0xC0

#define DMA_DTYP_68K  0x00
#define DMA_DTYP_6800 0x10
#define DMA_DTYP_ACK  0x20
#define DMA_DTYR_ACKRDY 0x30

#define DMA_DPS_8 0x00
#define DMA_DPS_16 0x08

#define DMA_PCL_READ_CSR 0x00
#define DMA_PCL_INT 0x01
#define DMA_PCL_PULSE 0x02
#define DMA_PCL_ABORT 0x03

/* OCR */
#define DMA_DIR_M2D 0x00
#define DMA_DIR_D2M 0x80

#define DMA_SIZE_BYTE 0x00
#define DMA_SIZE_WORD 0x10
#define DMA_SIZE_LONG 0x20
#define DMA_SIZE_UNPACKED 0x30

#define DMA_CHAIN_NONE 0x00
#define DMA_CHAIN_ARRAY 0x08
#define DMA_CHAIN_LIST 0x0C

#define DMA_REQG_LIMITED 0x00
#define DMA_REQG_MAXIMUM 0x01
#define DMA_REQG_EXTERNAL 0x02
#define DMA_REQG_AUTO 0x03

/* SCR */
#define DMA_MAC_SAME 0x00
#define DMA_MAC_UP 0x04
#define DMA_MAC_DOWN 0x08

#define DMA_DAC_SAME 0x00
#define DMA_DAC_UP 0x01
#define DMA_DAC_DOWN 0x02

/* CCR */
#define DMA_STR_START 0x80
#define DMA_CNT_CONTINUE 0x40
#define DMA_HLT_HALT 0x20
#define DMA_SAB_ABORT 0x10
#define DMA_INT_OK 0x08

typedef struct {
  void *source;
  unsigned short transfer_count;
} DMA_ChainBlock;

extern DMA_ChainBlock DMA_Chain[500];

#endif
