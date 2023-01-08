#include <apple2enh.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sp.h"

char url[256];
int8_t net;

char username[64], password[64];

void banner(void)
{
  clrscr();
  revers(1);
  cprintf(" WELCOME TO NETCAT ");
  revers(0);
  cprintf("\r\n\r\n");
}

bool getURL(void)
{
  cprintf("ENTER NETWORK URL:\r\n>> ");
  gets(url);

  return url[0]=='N' && url[1] == ':';
}

void getCreds(void)
{
  cprintf("LOGIN, OR <RETURN> FOR NONE:\r\n>> ");
  gets(username);

  if (username[0]!=0x00)
    {
      sp_payload[0]=strlen(username);
      sp_payload[1]=0;
      strcpy((char *)&sp_payload[2],username);
      sp_control(net,0xFD);
    }
  
  cprintf("\n\nPASSWORD, OR <RETURN> FOR NONE:\r\n>> ");
  gets(password);

  if (password[0]!=0x00)
    {
      sp_payload[0]=strlen(password);
      sp_payload[1]=0;
      strcpy((char *)&sp_payload[2],password);
      sp_control(net,0xFE);
    }
}
void connect(void)
{
  sp_open(net);
  sp_payload[0]=0x02; // 258 bytes
  sp_payload[1]=0x01;
  sp_payload[2]=0x0C; // READ/WRITE (or GET)
  sp_payload[3]=0x80; // NO TRANSLATION
  memcpy(&sp_payload[4],url,256);
  sp_control(net,'O'); // Do the open.  
}

bool isConnected(void)
{
  sp_status(net,'S');
  return sp_payload[2] & 0x02; // Bit 2 = Connected?
}

void in()
{
  unsigned short bw; // Bytes waiting
  unsigned short i;
  
  sp_status(net,'S');

  bw = sp_payload[0];
  bw |= (sp_payload[1]) << 8;

  if (bw==0)
    return;

  memset(sp_payload,0,sizeof(sp_payload));
  
  sp_read(net,bw);

  for (i=0;i<bw;i++)
    if (sp_payload[i]!='\r')
      putchar(sp_payload[i]);
}

void out()
{
  if (kbhit())
    {
      memset(sp_payload,0,sizeof(sp_payload));
      sp_payload[0]=1;
      sp_payload[1]=0;
      sp_payload[2]=cgetc();
      sp_control(net,'W');
    }
}

bool done(void)
{
  return false;
}

void main(void)
{
  videomode(VIDEOMODE_80x24);
  sp_init();
  net = sp_find_network();

  banner();
  
  cprintf("NET DEV IS %d\r\n",net);
  
  while(!getURL());

  getCreds();
  
  connect();

  cprintf("CONNECTED.\n");
  
  do 
    {
      in();
      out();
    }
  while (1);
}
