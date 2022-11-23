/**
 * DUMB Typewriter - Test the printer device
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "sp.h"

void main(void)
{
  unsigned char ptr;

  clrscr();

  sp_init();
  
  ptr = sp_find_printer();

  printf("Printer is dev %u\n\n",ptr);

  printf("DUMB TYPEWRITER - <RETURN> SENDS TO PTR\n\n");

  while(1)
    {
      gets((char *)sp_payload);
      sp_write(ptr,strlen((char *)sp_payload));
    }
}
