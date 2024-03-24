/**
 * @brief FujiNet Lobby for Adam
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <stdlib.h>
#include <smartkeys.h>
#include <conio.h>
#include <video/tms99x8.h>
#include "display.h"
#include "state.h"
#include "fetch.h"

extern State state;
extern char _username[32];
extern int numEntries;
extern FetchPage fetchPage[FETCH_NUM_PER_PAGE];

char tmp[8];

const char *display_status(int o)
{
  return (o ? "ONLINE" : "OFFLINE");
}

char *display_players(int p, int m)
{
  sprintf(tmp,"%u/%u",p,m);
  return tmp;
}

unsigned char display_offset_y_g(unsigned char i)
{
  switch(i)
    {
    case 0:
      return 8;
    case 1:
      return 32;
    case 2:
      return 56;
    case 3:
      return 80;
    case 4:
      return 104;
    }
}

unsigned char display_offset_y_s(unsigned char i)
{
  switch(i)
    {
    case 0:
      return 16;
    case 1:
      return 40;
    case 2:
      return 64;
    case 3:
      return 88;
    case 4:
      return 112;
    }
}

void display_username(void)
{
  vdp_color(15,4,7);
  vdp_vfill(0x00,0x00,0x0100); // Clear first line.
  gotoxy(0,0);
  cprintf("%32s",_username);
  smartkeys_puts(0,0,"#FUJINET GAME LOBBY");
}

void display(void)
{
  clrscr();
  
  display_username();
  
  gotoxy(0,1);

  vdp_color(1,15,7);

  // Pre-fill screen color for # of visible entries
  vdp_vfill(MODE2_ATTR+0x100,0x1F,0x300*numEntries);
  
  for (int i=0;i<numEntries;i++)
    {
      cprintf("%32s%32s%32s",display_status(fetchPage[i].o),display_players(fetchPage[i].p,fetchPage[i].m),"");
      smartkeys_puts(0,display_offset_y_g(i),fetchPage[i].g);
      smartkeys_puts(0,display_offset_y_s(i),fetchPage[i].s);
    }

  state=SELECT;
}
