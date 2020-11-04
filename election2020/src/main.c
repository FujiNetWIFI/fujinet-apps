/**
 * Election
 */

#include <atari.h>
#include <string.h>
#include <peekpoke.h>
#include <stdlib.h>
#include "screen.h"
#include "sio.h"

unsigned char trip=0;

unsigned char fontPatch[8]=
  {
   255,255,255,255,255,255,255,255
  };

void config_dlist=
  {
   DL_BLK8,
   DL_BLK8,
   DL_BLK8,
   DL_LMS(DL_CHR20x16x2),
   DISPLAY_MEMORY,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_JVB,
   0x600,
   0,0,0,0
  };

char buf[256];

char date[20];
char time[20];

char devicespec[256]="N:HTTPS://FUJINET.ONLINE/2020pres/";

extern unsigned char* video_ptr;
extern unsigned char* dlist_ptr;
extern unsigned short screen_memory;
extern unsigned char* font_ptr;

void election(unsigned short r, unsigned short d, char* date, char* time)
{
  char rs[4]={0,0,0,0};
  char ds[4]={0,0,0,0};

  unsigned char rp,dp,i;

  screen_clear();
  screen_puts(0,0,"   #FUJINET  2020   ");
  screen_puts(0,1,"  ELECTION RESULTS ");

  screen_puts(0,3,"trump");
  screen_puts(0,6,"\xA2\xA9\xA4\xA5\xAE");

  itoa(r,rs,10);
  itoa(d,ds,10);

  screen_puts(7,3,rs);
  screen_puts(7,6,ds);

  rp=r/16;
  dp=d/16;

  for (i=0;i<rp;i++)
    {
      screen_puts(i,4,"\x7A");
    }

  for (i=0;i<dp;i++)
    {
      screen_puts(i,7,"\xBA");
    }

  screen_puts(0,9,"    LAST UPDATED:   ");
  screen_puts(4,10,date);
  screen_puts(5,11,time);

  if (d>269)
    {
      screen_puts(12,3,"WINS!");
      for (;;) { OS.color1++; }
    }
  else if (r>269)
    {
      screen_puts(12,6,"WINS!");
      for (;;) { OS.color2++; }
    }
  
}

void get_data(unsigned short *r, unsigned short *d, char *date, char* time)
{
  char* token;
  char i;

  screen_clear();
  screen_puts(0,5,"   FETCHING DATA    ");

  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd='O';
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=&devicespec;
  OS.dcb.dtimlo=0x1F;
  OS.dcb.dbyt=256;
  OS.dcb.daux1=4;
  OS.dcb.daux2=3; // CR/LF
  siov();
  OS.rtclok[0]=OS.rtclok[1]=OS.rtclok[2]=0;

  while (OS.rtclok[2]<250) { }

  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd='S';
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=OS.dvstat;
  OS.dcb.dtimlo=0x1F;
  OS.dcb.dbyt=4;
  OS.dcb.daux1=0;
  OS.dcb.daux2=0; // CR/LF
  siov();

  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd='R';
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=buf;
  OS.dcb.dtimlo=0x1F;
  OS.dcb.dbyt=OS.dvstat[0];
  OS.dcb.daux=OS.dvstat[0];
  siov();

  // Parse it.

  token=strtok(buf,"|");
  while (token != NULL)
    {
      if (i==1)
	*d=atoi(token);
      else if (i==3)
	*r=atoi(token);
      else if (i==4)
	strcpy(date,token);
      else if (i==5)
    strcpy(time,token);
      i++;

      token=strtok(NULL,"|");
    }

  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd='C';
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=0;
  OS.dcb.dtimlo=0x1F;
  OS.dcb.dbyt=0;
  OS.dcb.daux1=0;
  OS.dcb.daux2=0; // CR/LF
  siov();
}

void setup(void)
{
  OS.color0=0x0f;
  OS.color1=0x46;
  OS.color2=0x96;
  OS.color4=0x90;
  OS.coldst=1;

  memcpy((void *)DISPLAY_LIST,&config_dlist,sizeof(config_dlist)); // copy display list to $0600
  OS.sdlst=(void *)DISPLAY_LIST;                     // and use it.
  dlist_ptr=(unsigned char *)OS.sdlst;               // Set up the vars for the screen output macros
  screen_memory=PEEKW(560)+4;
  video_ptr=(unsigned char*)(PEEKW(screen_memory));

  // Copy ROM font
  memcpy((unsigned char *)FONT_MEMORY,(unsigned char *)0xE000,1024);

  /* // And patch it. */
  font_ptr=(unsigned char*)FONT_MEMORY;
  memcpy(&font_ptr[464],&fontPatch,sizeof(fontPatch));
  OS.chbas=0x78; // use the charset
}

void main(void)
{
  unsigned short r,d;

  setup();

  for (;;)
    {
      get_data(&r,&d,date,time);
      election(r,d,date,time);

      OS.rtclok[0]=OS.rtclok[1]=OS.rtclok[2]=0;
      while (OS.rtclok[1]<0x56) { }
    }
}
