/**
 * @brief   Light Cycles example game
 * @verbose Shows example of two player networked game with #FujiNet
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 */

#include <apple2.h>
#include <tgi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include "nio.h"
#include "die.h"
#include "sp.h"

unsigned char net; /* SmartPort Network Device ID */
char my_name[12], other_name[12], hostname[64];
unsigned short bw; /* bytes waiting to read */
unsigned char error; /* network error code */
bool connected; /* Are we connected? */

/**
 * @brief setup graphics
 */
void setup(void)
{
  if (!sp_init())
    die("SMARTPORT NOT FOUND");

  net = sp_find_network();

  if (!net)
    die("NETWORK DEVICE NOT FOUND");

  /* Everything ok, set up mixed lo-res graphics. */
  
  tgi_install(a2_lo_tgi);
  tgi_init();
  tgi_apple2_mix(1);
  tgi_clear();
}

/**
 * @brief Plot a double high pixel
 */
void plot(unsigned char x, unsigned char y)
{
  y <<= 1; // Double
  tgi_setpixel(x,y);
  tgi_setpixel(x,y+1);
}

/**
 * @brief draw the board
 */
void draw_board(void)
{
  unsigned char i;
  
  tgi_clear();

  tgi_setcolor(TGI_COLOR_BLUE);
  
  for (i=0;i<40;i++)
    plot(i,0);

  for (i=0;i<20;i++)
    plot(39,i);

  for (i=0;i<40;i++)
    plot(i,19);

  for (i=0;i<20;i++)
    plot(0,i);
}

/**
 * @brief Get the player name
 */
void get_player(void)
{
  revers(1);

  printf(" #FUJINET LIGHT CYCLES \n\n");

  revers(0);
  
  printf("WHAT IS YOUR NAME? ");
  gets(my_name);
}

/**
 * @brief get the host name
 */
void get_host(void)
{
  printf("ENTER HOST NAME, OR ");
  revers(1);
  printf(" RETURN ");
  revers(0);
  printf(" TO LISTEN:\n");
  gets(hostname);
}

/**
 * @brief listen for connection, if hostname empty.
 */
void listen(void)
{
  if (nopen(1,"N:TCP://:6502/",0x0C,0x00))
    die("COULD NOT OPEN LISTENING CONNECTION");

  printf("LISTENING ON PORT 6502...");
  
  while (!connected)
    nstatus(1,&bw,&connected,&error);
  
}

/**
 * @brief establish TCP connection to host
 */
void connect(void)
{
}

void main(void)
{
  setup();  
  draw_board();

  get_player();
  get_host();

  if (hostname[0]=='\0')
    listen();
  else
    connect();
  
  for(;;);
}
