/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifdef BUILD_ADAM

#include <smartkeys.h>
#include <eos.h>
#include <msx.h>
#include <conio.h>
#include <stdbool.h>

#include "screen.h"

#define ROW(x) (x<<8)
#define COL(x) (x<<3)

#define ATTR_BKG 0x1F
#define ATTR_BAR 0x13;
#define ATTR_COL 0xF5;

void screen_init(void)
{
  smartkeys_sound_init();
  smartkeys_set_mode();
}

void screen_select_host(HostSlots *hs)
{
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("  WELCOME TO RESTORE-IMAGE\n  PLEASE SELECT A HOST SLOT\n  AND PRESS [RETURN]");

  msx_vfill(MODE2_ATTR,0xF4,256);
  msx_vfill(MODE2_ATTR+256,0x1F,1024);
  
  gotoxy(0,0); msx_color(15,4,7); cprintf("%31s","HOST SLOTS");

  for (unsigned char i=0;i<8;i++)
    cprintf("%d%s",i+1,hs[i]);

  msx_vfill_v(MODE2_ATTR,0xF4,40);
}

void screen_bar(char y, char c, char m, char i, bool onOff)
{
  msx_vfill(MODE2_ATTR + ROW(y+i), (onOff == true ? 0x13 : 0xF5), COL(c));
  msx_vfill(MODE2_ATTR + ROW(y+i), (onOff == true ? 0x13 : 0x1F), 256 - COL(c));
}

#endif /* BUILD_ADAM */
