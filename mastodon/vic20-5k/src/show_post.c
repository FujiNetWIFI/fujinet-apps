/**
 * Show latest Mastodon post from OLDBYTES.SPACE
 *
 * @author   Thomas Cherryhomes
 * @email    thom dot cherryhomes at gmail dot com
 * @license  gpl v. 3
 */

#include <vic20.h>
#include <peekpoke.h>
#include <conio.h>
#include "video.h"

extern char display_name[23], created_at[23], content[400];

/**
 * @brief Show Post
 */
void show_post(void)
{
  unsigned short i;

  clrscr();
  print("\x1F\x12 LATEST MASTODON POST \x92\n\x1C");
  gotoxy(0,2);
  print(display_name);
  gotoxy(0,3);
  print("\x9C");
  print(created_at);
  print("\n\x90");
  print(content);

  POKE(0xCC,0);
  
  for (i=0;i<16000;i++)
    waitvsync();
}
