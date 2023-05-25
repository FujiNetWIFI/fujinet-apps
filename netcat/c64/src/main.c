/**
 * NetCat - a dumb terminal for #FujiNet, C64 version
 *
 * @author   Thomas Cherryhomes
 * @email    thom dot cherryhomes gmail dot com
 * @license  gpl v. 3
 */

#include <c64.h>
#include <cbm.h>
#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <peekpoke.h>

#define LFN 2     // The logical file number to use for I/O
#define DEV 12    // The network device #
#define SAN 2     // The secondary address (SA) to use on DEV.
#define CMD 15    // The secondary address for the Command channel

char buf[4096];   // buffer. reused for everything.
char settrans_cmd[] = "settrans,2,0"; // set PETSCII translation

bool trip=false;
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

  POKE(0xCC,0); // Cursor.  
}

/**
 * @brief main program
 */
void main(void)
{
  // Set up colors
  textcolor(COLOR_LIGHTGREEN);
  bordercolor(COLOR_BROWN);
  bgcolor(COLOR_BLACK);
  clrscr();

  // Show banner
  cprintf("NETCAT:  AN EXAMPLE TERMINAL FOR FUJINET");
  gotoxy(0,2);
  textcolor(COLOR_LIGHTRED);
  cprintf("URL >>> ");
  gets(buf);

  textcolor(COLOR_WHITE);
  bordercolor(COLOR_BLUE);
  clrscr();

  ihsetup();

  // Open connections to cmd and endpoint
  cbm_open(CMD,DEV,CMD,"");
  cbm_write(CMD,settrans_cmd,sizeof(settrans_cmd)); // Set ASCII<->PETSCII translation
  cbm_open(LFN,DEV,SAN,buf);
  
  while(1)
    {
      out();
      in();
    }
  
  // Close connections to cmd and endpoint
  cbm_close(LFN);
  cbm_close(CMD);
}
