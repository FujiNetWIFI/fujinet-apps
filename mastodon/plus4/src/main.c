/**
 * Show latest Mastodon post from OLDBYTES.SPACE
 *
 * @author   Thomas Cherryhomes
 * @email    thom dot cherryhomes at gmail dot com
 * @license  gpl v. 3
 */

#include "fetch_post.h"
#include "show_post.h"

char display_name[40];
char created_at[40];
char content[800];
int  replies_count;
int  reblogs_count;
int  favorites_count;

void main(void)
{
  int i=0;
  
  while(1) // Run continuously.
    {
      fetch_post(display_name,
		 created_at,
		 content,
		 &replies_count,
		 &reblogs_count,
		 &favorites_count);

      show_post(display_name,
		created_at,
		content,
		replies_count,
		reblogs_count,
		favorites_count);


      for (i=0;i<262144;i++); // Delay in lieu of no clock funcs.
      
    }
}
