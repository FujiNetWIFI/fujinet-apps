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

/**
 * @brief Show Post
 * @param display_name The name to display (must be 40 chars, or less)
 * @param created_at The created at timestamp to display (must be 40 chars, or less)
 * @param content The content to display, must be 800 chars or less)
 * @param replies_count # of replies to thread
 * @param reblogs_count # of boosts
 * @param favorites_count # of hearts
 */
void show_post(const char *display_name,
	       const char *created_at,
	       const char *content)
{
  unsigned short i;
  
  print("\x93\x12LATEST  MASTODON  POST\x92");
  
  gotoxy(0,2);
  print(display_name);
  print("\x0d");
  print(created_at);
  print("\x0d\x90");
  cprintf(content);

  POKE(0xCC,0);
  
  for (i=0;i<16000;i++)
    waitvsync();
}
