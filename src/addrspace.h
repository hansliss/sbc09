#ifndef _MEMSPACE_H
#define _MEMSPACE_H

#include "v09.h"

#define RAMSIZE 0x8000
#define MEMSIZE 0x10000
#define IOPAGE 0xa000

extern void initMem();
extern void read_image(char *imageName, int offset);
extern Byte getMem(Word addr);
extern void setMem(Word addr, Byte value);

#endif
