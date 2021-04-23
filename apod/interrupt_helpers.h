#ifndef INTERRUPT_HELPERS_H
#define INTERRUPT_HELPERS_H

/* Keep track of old VBI vector, so we can jump to it at
   the end of ours (see below), and restore it when we're done
   needing our VBI */
extern void * OLDVEC;

void mySETVBV(void * Addr);
void dli_init(void * dli_func);
void dli_clear(void);

#endif // INTERRUPT_HELPERS_H

