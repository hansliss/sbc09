/* 6809 Simulator addrspace module
   created 2020 by Hans Liss
   license: GNU General Public License version 2, see LICENSE for more details.

   This module simulates an 6809 address space.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "v09.h"
#include "io.h"
#include "addrspace.h"

#define A0(addr) ((addr) & 0x0001)
#define A1(addr) ((addr) & 0x0002)
#define A2(addr) ((addr) & 0x0004)
#define A3(addr) ((addr) & 0x0008)
#define A4(addr) ((addr) & 0x0010)
#define A5(addr) ((addr) & 0x0020)
#define A6(addr) ((addr) & 0x0040)
#define A7(addr) ((addr) & 0x0080)
#define A8(addr) ((addr) & 0x0100)
#define A9(addr) ((addr) & 0x0200)
#define A10(addr) ((addr) & 0x0400)
#define A11(addr) ((addr) & 0x0800)
#define A12(addr) ((addr) & 0x1000)
#define A13(addr) ((addr) & 0x2000)
#define A14(addr) ((addr) & 0x4000)
#define A15(addr) ((addr) & 0x8000)

typedef struct _signals {
  int RAM;
  int ROM;
  int IO[16];
} signals;

int AND4(int in0, int in1, int in2, int in3) {
  return in0 && in1 && in2 && in3;
}

int AND2(int in0, int in1) {
  return in0 && in1;
}

int NOT(int i0) {
  return !i0;
}

int NAND2(int in0, int in1) {
  return !AND2(in0, in1);
}

int OR2(int in0, int in1) {
  return in0 || in1;
}

void decode4to16(int e0, int e1, int a0, int a1, int a2, int a3, int out[16]) {
  for (int i=0; i<16; i++) {
    out[i] = 1;
  }
  if (!e0 && !e1) {
    int addr = (a0?1:0) | (a1?2:0) | (a2?4:0) | (a3?8:0);
    out[addr] = 0;
  }
}

void assertCorrectDecode(signals *sig) {
  int actCount=0;
  if (!(sig->RAM)) {
    actCount++;
  }
  if (!(sig->ROM)) {
    actCount++;
  }
  for (int i=0; i<16; i++) {
    if (!(sig->IO[i])) {
      actCount++;
    }
  }
  assert(actCount == 1);
}

void dumpDecoded(signals *sig) {
  if (!(sig->RAM)) {
    fprintf(stderr, "RAM\n");
  }
  if (!(sig->ROM)) {
    fprintf(stderr, "ROM\n");
  }
  for (int i=0; i<16; i++) {
    if (!(sig->IO[i])) {
      fprintf(stderr, "IO%d\n", i);
    }
  }
}

void adecode(Word addr, signals *sig) {
  int nib1 = AND4(A4(addr), A5(addr), A6(addr), A7(addr));
  int nib2 = AND4(A8(addr), A9(addr), A10(addr), A11(addr));
  int nib3 = AND4(A12(addr), A13(addr), A14(addr), A15(addr));
  int TOPWIN = AND2(nib2, nib3);
  int notVECTORS = NAND2(nib1, TOPWIN);
  int notIO = NAND2(notVECTORS, TOPWIN);
  sig->RAM = AND2(A14(addr), A15(addr));
  sig->ROM = NAND2(notIO, sig->RAM);
  int notIOBLOCK0 = OR2(A6(addr), A7(addr));
  decode4to16(notIO, notIOBLOCK0, A2(addr), A3(addr), A4(addr), A5(addr), sig->IO);
  assertCorrectDecode(sig);
}
  
/* 6809 memory space */
Byte * mem;

void initMem() {
  if((mem=calloc(1, MEMSIZE))==0) { 
    fprintf(stderr,"Not enough memory\n");
    exit(2);
  } 
}

void read_image(char *imageName, int offset)
{
 FILE *image;
 if((image=fopen(imageName, "rb"))==NULL) {
   perror(imageName);
   exit(2);
 }
 if (fread(mem + offset, MEMSIZE - offset, 1, image) != 1) {
   perror("fread()");
   exit(-1);
 }
 fclose(image);
}


Byte getMem(Word addr) {
  signals decoded;
  adecode(addr, &decoded);
  if (!(decoded.IO[0])) {
    return do_input(addr & 0x03);
  } else {
    return mem[addr];
  }
}

void setMem(Word addr, Byte value) {
  signals decoded;
  adecode(addr, &decoded);
  if (!(decoded.IO[0])) {
    do_output(addr & 0x03, value);
  } else if (!(decoded.RAM)) {
    mem[addr] = value;
  } else if (!(decoded.ROM)) {
    fprintf(stderr, "Write to ROM (%04X) at %04X\n", addr, pcreg);
    restore_term();
    exit(3);
  } else {
    restore_term();
    dumpDecoded(&decoded);
    exit(3);
  }
}


