#ifndef _IO_H
#define _IO_H

extern void restore_term();
extern void set_term(char c);

extern int do_input(int);
extern void do_output(int,int);
extern void do_escape(void);

#endif
