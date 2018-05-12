#ifndef DRAWPORT_H
#define DRAWPORT_H

#include "fastdraw/types.h"

typedef enum {
  DEPTH_CHUNKY = -2,
  DEPTH_TEXT = -1,
  DEPTH_1BPP = 1,
  DEPTH_2BPP = 2,
  DEPTH_4BPP = 4  
} DRAWPORT_DEPTH;

typedef struct drawport_t {
  uint8_t *vram_base;
  Point size;
  uint8_t depth;
  uint8_t padding; //10 bytes long
} DrawPort;

extern DrawPort screenPort;
extern DrawPort *currentPort; // the current drawing port

#endif
