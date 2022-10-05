/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ETX 0x03
#define BS 0x08
#define LF 0x0A

bool _input_pw=false;

void input(char *s)
{
  char k=0;
  int i=0;
  
  while (1)
    {
      if (_input_pw==true)
	{
	  k=cgetc();
	  putchar('*');
	}
      else
	k=getchar();

      if (k==ETX)
	{
	  printf("^C");
	  exit(1);
	}
      else if (k==LF)
	break;
      else if ((k==BS) && (i>0))
	s[i--]=0x00;
      else if ((k<0x20) && (k>0x7f)) // non-printable
	continue;
      else
	s[i++]=k;
    }
}
