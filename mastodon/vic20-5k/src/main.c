/**
 * Show latest Mastodon post from OLDBYTES.SPACE
 *
 * @author   Thomas Cherryhomes
 * @email    thom dot cherryhomes at gmail dot com
 * @license  gpl v. 3
 */

#include <conio.h>
#include "fetch_post.h"
#include "show_post.h"

char display_name[23];
char created_at[23];
char content[400];

void main(void)
{
  clrscr();
  
  while(1) // Run continuously.
    {
      fetch_post();
      show_post();
    }
}
