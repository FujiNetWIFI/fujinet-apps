/**
 * @brief SRQ test
 */

#include <vic20.h>
#include <cbm.h>
#include <conio.h>
#include <stdbool.h>
#include <peekpoke.h>
#include "video.h"

bool trip;
bool running=true;

char buf[512];   // buffer. reused for everything.

extern void ihsetup();
extern void ihdone();

void out(void)
{
  unsigned char i=0,c=0;
  
  while(kbhit())
    {
      cursor(1);
      c = cgetc();
      if (c == CH_F1)
	running=false;
      else
	buf[i++] = c;
      cursor(1);
    }

  if (!i)
    return;

  cbm_write(2,buf,i);
}

void in(void)
{
  int i=0, l=0;

  if (trip)
    {
      l = cbm_read(2,buf,sizeof(buf));

      cursor(1);
      for (i=0;i<l;i++)
	cbm_k_bsout(buf[i]);
      cursor(1);
      
      trip = false;
    }
}

void main(void)
{
  ihsetup();
  
  clrscr();

  print("\x12 NETCAT \x92\r\nENTER URL\x0D> ");
  input(buf);

  print("\x0D""CONNECTING TO:\x0d\x0d");
  print(buf);
  print("\x0d\x0d\x12 F1 \x92 TO QUIT.\x0d\x0d");

  cbm_open(2,12,2,buf);

  while (running)
    {
      out();
      in();
    }

  cbm_close(2);
  
  ihdone();

  print("BYE.\r\n\r\n");
}
