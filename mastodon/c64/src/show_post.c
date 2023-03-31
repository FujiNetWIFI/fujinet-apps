/**
 * Show latest Mastodon post from OLDBYTES.SPACE
 *
 * @author   Thomas Cherryhomes
 * @email    thom dot cherryhomes at gmail dot com
 * @license  gpl v. 3
 */

#include <c64.h>
#include <conio.h>

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
	       const char *content,
	       int replies_count,
	       int reblogs_count,
	       int favorites_count)
{
  bgcolor(COLOR_BLACK);
  bordercolor(COLOR_BLUE);
  clrscr();

  textcolor(COLOR_GRAY3);
  revers(1);
  cprintf("LATEST MASTODON POST FROM OLDBYTES.SPACE");
  revers(0);
  
  gotoxy(0,2);
  textcolor(COLOR_LIGHTBLUE);
  cprintf("%40s",display_name);
  textcolor(COLOR_LIGHTGREEN);
  cprintf("%40s",created_at);

  gotoxy(0,5);
  textcolor(COLOR_YELLOW);
  cprintf(content);

  gotoxy(0,23);
  textcolor(COLOR_LIGHTRED);
  cprintf("LIKES: ");
  textcolor(COLOR_YELLOW);
  cprintf("%-4u ",favorites_count);

  textcolor(COLOR_LIGHTBLUE);
  cprintf("REPLIES: ");
  textcolor(COLOR_YELLOW);
  cprintf("%-4u ",replies_count);

  textcolor(COLOR_CYAN);
  cprintf("BOOSTS: ");
  textcolor(COLOR_YELLOW);
  cprintf("%-4u ",reblogs_count);
}
