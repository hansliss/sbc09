/* v09.h
   This file is part of the 6809 simulator v09

   created 1994 by L.C. Benschop.
   copyleft (c) 1994-2014 by the sbc09 team, see AUTHORS for more details.
   license: GNU General Public License version 2, see LICENSE for more details.

*/

#ifndef _V09_H
#define _V09_H

typedef unsigned char Byte;
typedef unsigned short Word;

/* 6809 registers */
extern Byte ccreg,dpreg;
extern Word xreg,yreg,ureg,sreg,ureg,pcreg;

extern Byte d_reg[2];
extern Word *dreg;
extern Byte *breg,*areg;

extern volatile int tracing,attention,escape,irq;
extern Word tracehi,tracelo;
extern char escchar;

extern void do_exit(void);
extern void do_trace(void);

#endif
