/**
 * Simple conio for E:
 */

#include <atari.h>
#include <stdlib.h>
#include <string.h>
#include "cio.h"

void printl(const char* c, int l)
{
  OS.iocb[0].buffer=(unsigned char *)c;
  OS.iocb[0].buflen=l;
  OS.iocb[0].command=IOCB_PUTCHR;
  ciov();
}

void printc(char* c)
{
  OS.iocb[0].buffer=c;
  OS.iocb[0].buflen=1;
  OS.iocb[0].command=IOCB_PUTCHR;
  ciov();
}

void print(const char* c)
{
  int l=strlen(c);
  printl(c,l);
}

void print_error(unsigned char err)
{
  char tmp[6];
  bzero(tmp,sizeof(tmp));
  itoa(err,tmp,10);
  print(tmp);
  print("\x9b");
}

void get_line(char* buf, unsigned char len)
{
  OS.iocb[0].buffer=buf;
  OS.iocb[0].buflen=len;
  OS.iocb[0].command=IOCB_GETREC;
  ciov();
}
