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
#include <sys/ioctl.h>

#include "screen.h"

#define ROW(x) (x<<8)
#define COL(x) (x<<3)

#define ATTR_BKG 0x1F
#define ATTR_BAR 0x13;
#define ATTR_COL 0xF5;

extern char hs[8][32];

static char udg[] =
  {
   0,0,0,0,0,0,3,51,                               // WIFI 1
   0,0,3,3,51,51,51,51,                            // WIFI 2
   48,48,48,48,48,48,48,48,                        // WIFI 3
   0x00,0x07,0x08,0x0F,0x0F,0x0F,0x0F,0x00,        // FOLDER 1 0x83
   0x00,0x80,0x70,0xF0,0xF0,0xF0,0xF0,0x00,        // FOLDER 2 0x84
   0x0F,0x08,0x08,0x0A,0x08,0x08,0x0F,0x00,        // DDP 1    0x85
   0xF0,0x10,0x10,0x50,0x10,0x10,0xF0,0x00,        // DDP 2    0x86
   0x0F,0x08,0x08,0x08,0x0A,0x08,0x0F,0x00,        // DSK 1    0x87
   0xF0,0x10,0xD0,0xD0,0xD0,0x10,0xF0,0x00,        // DSK 2    0x88
   0x0F,0x08,0x0B,0x0B,0x0B,0x08,0x0F,0x00,        // ROM 1    0x89
   0xF0,0x10,0xD0,0xD0,0xD0,0x10,0xF0,0x00,        // ROM 2    0x8a
   0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55  // Password smudge 0x8b
  };

void screen_init(void)
{
  void *param = &udg;
  
  msx_color(0,0,0);

  console_ioctl(IOCTL_GENCON_SET_UDGS,&param);
  smartkeys_set_mode();
}

void screen_select_host(void)
{
  smartkeys_set_mode();
  clrscr();
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("  WELCOME TO RESTORE-IMAGE\n  PLEASE SELECT A HOST SLOT\n  AND PRESS [RETURN]");

  msx_vfill(MODE2_ATTR,0xF4,256);
  msx_vfill(MODE2_ATTR+256,0x1F,2048);
  
  gotoxy(0,0); msx_color(15,4,7); cprintf("%32s","HOST SLOTS");

  msx_color(1,15,7);

  for (unsigned char i=0;i<8;i++)
    cprintf("%d%s\n",i+1,hs[i]);

  msx_vfill_v(MODE2_ATTR,0xF4,72);
}

void screen_bar(char y, char c, char m, char i, bool onOff)
{
  msx_vfill(MODE2_ATTR + ROW(y+i), (onOff == true ? 0x13 : 0xF5), COL(c));
  msx_vfill(MODE2_ATTR + ROW(y+i) + COL(c), (onOff == true ? 0x13 : 0x1F), 256 - COL(c));
}

void screen_select_file(void)
{
  msx_vfill(0x0000,0x00,5120);
  
  msx_color(15,4,7);
  msx_vfill(MODE2_ATTR,0xF4,512);

  // Paint content area
  msx_vfill(MODE2_ATTR+0x200,0xF5,256);
  msx_vfill(MODE2_ATTR+0x300,0x1F,0x0F00);
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("  OPENING...");
}

void screen_select_file_display(char *p, char *f,char *hsn)
{
  // Clear content area
  msx_vfill(0x0000,0x00,0x1400);
  msx_vfill(MODE2_ATTR+0x0100,0xF5,256);
  msx_vfill(MODE2_ATTR+0x1200,0xF5,256);
  msx_vfill_v(MODE2_ATTR+0x0200,0xF5,136);
  msx_vfill_v(MODE2_ATTR+0x0200+8,0xF5,136);
  
  // Update content area
  msx_color(15,4,7);
  gotoxy(0,0); cprintf("%32s", hsn);

  if (f[0]==0x00)
    cprintf("%32s",p);
  else
    cprintf("%22s|%8s|",p,f);
}

void screen_select_file_display_entry(unsigned char y, char* e)
{
  gotoxy(0,y+3);
  msx_color(15,5,7);
  cprintf("%c%c",*e++,*e++);
  msx_color(1,15,7);
  cprintf("%-30s",e);
  //smartkeys_sound_play(SOUND_TYPEWRITER_CLACK);
}

void screen_select_file_prev(void)
{
  msx_color(1,5,7);
  gotoxy(0,2); cprintf("%32s","[...]");
}

void screen_select_file_next(void)
{
  msx_color(1,5,7);
  gotoxy(0,18); cprintf("%32s","[...]");
}

void screen_select_file_display_long_filename(char *e)
{
  gotoxy(0,19);
  cprintf("%-64s",e);
}

void screen_select_file_clear_long_filename(void)
{
  gotoxy(0,0);
  msx_vfill(0x1300,0,512);
}

void screen_select_file_choose(char visibleEntries)
{  
  smartkeys_display(NULL,NULL,NULL,(strcmp(path,"/") == 0) ? NULL: "   UP"," FILTER", "  SELECT");
  smartkeys_status("  SELECT SOURCE FILE.\n  [ESC] ABORT");
  
  // smartkeys_sound_play(SOUND_MODE_CHANGE);
}

void screen_select_file_filter(void)
{
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("  ENTER A WILDCARD FILTER.\n  E.G. *Coleco*");
  // smartkeys_sound_play(SOUND_POSITIVE_CHIME);
}

void screen_select_tape(void)
{
  smartkeys_display(NULL,NULL,NULL,NULL,"  TAPE\n   1", "  TAPE\n   2");
  smartkeys_status("  SELECT\n  DESTINATION.");
  // smartkeys_sound_play(SOUND_POSITIVE_CHIME);
}

void screen_perform_confirm(void)
{
  smartkeys_display(NULL,NULL,NULL,NULL,NULL," PERFORM\n RESTORE");
  smartkeys_status("  PLACE MEDIA IN DRIVE.");
}

void screen_perform_read(unsigned long blockNum)
{
  gotoxy(0,19); cprintf("%10lu%-22s",blockNum," BLOCKS READ");
}

void screen_perform_write(unsigned long blockNum)
{
  gotoxy(0,19); cprintf("%10lu%-22s",blockNum," BLOCKS WRITTEN");
  csleep(5);
}

void screen_perform_error(void)
{
  smartkeys_display(NULL,NULL,NULL,NULL,"    NO","   YES");
  smartkeys_status("  FAILED, RETRY?");
}

void screen_perform_verify_read(unsigned long blockNum)
{
  gotoxy(0,19); cprintf("%10lu%-22s",blockNum," BLOCKS RE-READ");
}

void screen_perform_verify_check(void)
{
  gotoxy(0,19); cprintf("%10s%-22s","","CHECKING");
}

void screen_perform_abort(void)
{
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("  OPERATION ABORTED.");
}

void screen_perform_restore(void)
{
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("  PERFORMING RESTORE");
}

void screen_perform_done(void)
{
  smartkeys_display(NULL,NULL,NULL,NULL,"   NO","   YES");
  smartkeys_status("  RESTORE DONE!\n  DO SAME TAPE AGAIN?");
}

#endif /* BUILD_ADAM */
