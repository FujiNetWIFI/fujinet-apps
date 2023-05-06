/**
 * @brief petsciiterm terminal
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details
 */

#include <c64.h>
#include <cbm.h> // For kernal I/O
#include <stdbool.h>
#include <conio.h>
#include <stdio.h>

#define LFN 2     // The logical file number to use for I/O
#define DEV 12    // The network device #
#define SAN 2     // The secondary address (SA) to use on DEV.
#define CMD 15    // The secondary address for the Command channel

extern char dial_url[40];
static bool connected;

char buf[4096];   // buffer. reused for everything.

bool trip=false;       // do we need to receive? 
extern void ihsetup(); // SRQ interrupt handler setup

/**
 * @brief Connect to host
 */
bool connect(void)
{
  textcolor(COLOR_WHITE);
  cputc('\xB6');
  revers(true);
  cputs("F1");
  revers(false);
  cputc('\xB5');
  cprintf("TO DISCONNECT\r\n\r\n");
  cprintf("CONNECTING TO:\r\n%s\r\n\r\n",dial_url);

  connected = true;
  
  return true;
}

/**
 * @brief Check if we're connected
 */
void check_connected(void)
{
  connected=true;
}

/**
 * @brief Network->Screen
 */
void in(void)
{
  int i=0, l=0;

  if (trip) // set by srq.s when SRQ asserted
    {
      l = cbm_read(LFN,buf,sizeof(buf));
      
      for (i=0;i<l;i++)
	putchar(buf[i]);

      trip = false;
    }
}

/**
 * @brief Keyboard->Network
 */
void out(void)
{
  unsigned char i=0;
  
  while(kbhit())
    buf[i++] = cgetc();
  
  if (!i)
    return;
  
  cbm_write(LFN,buf,i);
}

/**
 * @brief Main terminal
 */
void term(void)
{
  clrscr();
  
  if (!connect())
    return;

  while (connected)
    {
      check_connected();
      in();
      out();
    }
}
