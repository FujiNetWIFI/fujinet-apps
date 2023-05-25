/**
 * cpmterm - Quick and dirty test dumb terminal for cp/m.
 *
 * @author   Thomas Cherryhomes
 * @email    thom dot cherryhomes gmail dot com
 * @license  gpl v. 3
 */

#include <c64.h>
#include <cbm.h>
#include <stdio.h>
#include <conio.h>
#include <peekpoke.h>
#include <stdbool.h>

#define LFN 2     // The logical file number to use for I/O
#define DEV 14    // The network CPM
#define SAN 2     // The secondary address (SA) to use on DEV.

char buf[4096];   // buffer. reused for everything.

unsigned char trip=false;
extern void ihsetup();

/**
 * @brief handle keyboard->out
 */
void out(void)
{
  int i=0;
  
  while(kbhit())
    buf[i++] = cgetc();

  if (!i)
    return;

  cbm_write(LFN,buf,i);
}

/**
 * @brief handle in->display
 */
void in(void)
{
  int i=0, l=0;

  if (trip)
    {
      l = cbm_read(LFN,buf,sizeof(buf));
      
      for (i=0;i<l;i++)
	putchar(buf[i]);

      trip = false;
    }
}

/**
 * @brief main program
 */
void main(void)
{
  clrscr();

  // Open connections to cmd and endpoint
  cbm_open(LFN,DEV,SAN,"BOOTCPM");

  ihsetup();
  
  while(1)
    {
      out();
      in();
    }
  
  // Close connections to cmd and endpoint
  cbm_close(LFN);
}
