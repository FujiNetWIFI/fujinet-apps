/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Input line routine
 */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BS 0x08
#define RETURN 0x0D
#define DEL 0x7F
#define SPACE 0x20
#define CURSOR 0xA0

void input(char *s)
{
  char k=0;
  int i=0;
  unsigned char x=wherex();
  unsigned char y=wherey();
  
  while (true)
    {
      cputcxy(x,y,CURSOR);
      k=cgetc();
      
      if ((k==BS || k==DEL) && i>0)
	{
	  cputcxy(x,y,SPACE);
	  cputcxy(--x,y,CURSOR);
	  s[i--]=0x00;
	}
      else if(k==RETURN)
	break;
      else
	{
	  cputcxy(x++,y,k);
	  s[i++]=k;
	}
    }
}
