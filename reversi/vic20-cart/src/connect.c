/**
 * @brief #FujiNet Reversi
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <vic20.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include "video.h"
#include "status.h"

#define LFN 2
#define CMD 15
#define DEV 16    // The network device #
#define SAN 2

#define LISTEN_URL "tcp://:6502/"

char hostname[80];
char url[80];

bool listen=false;

void connect_listen(void)
{
  unsigned short bw;
  unsigned char connected, error;
  const char *accept="A,2,12,2";

  cbm_open(LFN,DEV,SAN,hostname);
  
  print(" WAITING FOR CONNECTION...");
  cursor(1);

  strcpy(url,LISTEN_URL);
  cbm_open(LFN,DEV,SAN,url);
  cbm_open(CMD,DEV,CMD,"");

  while(true)
    {
      if (!status(&bw,&connected,&error)) // connection error
	{
	  print("\n listening socket error\n halted.");
	  cbm_close(LFN);
	  cbm_close(CMD);
	  while(true);
	}

      if (connected)
	{
	  print("\n connected!\n");
	  break;
	}
    }

  print("accepting...\n");
  cbm_write(CMD,accept,sizeof(accept));
  
  cbm_close(CMD);
}

void connect_host(void)
{
  print(" connecting...");

  
}

void connect(void)
{
  VIC.bg_border_color = 104;
  clrscr();

  print("\x90\x12   #fujinet reversi   ");
  print("\x92\n\x05"" enter host name, or\n \x12 return \x92 to listen.\n");

  gotoxy(1,5);
  input(hostname);

  if (!hostname[0])
    {
      listen=true;
      connect_listen();
    }
  else
    {
      connect_host();
    }
}

void main(void)
{
  connect();
  while(1);
}
