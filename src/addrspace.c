/* 6809 Simulator addrspace module
   created 2020 by Hans Liss
   license: GNU General Public License version 2, see LICENSE for more details.

   This module simulates an 6809 address space.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v09.h"
#include "addrspace.h"

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
  if ((addr & 0xff00) != IOPAGE) {
    return mem[addr];
  } else {
    return do_input(addr & 0xff);
  }
}

void setMem(Word addr, Byte value) {
  if ((addr & 0xff00) != IOPAGE) {
    if (!(addr & 0x8000)) {
      mem[addr] = value;
    }
  } else {
    do_output(addr & 0xff, value);
  }
}


