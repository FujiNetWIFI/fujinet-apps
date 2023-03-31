/**
 * Show latest Mastodon post from OLDBYTES.SPACE
 *
 * @author   Thomas Cherryhomes
 * @email    thom dot cherryhomes at gmail dot com
 * @license  gpl v. 3
 */

#include <time.h>
#include "fetch_post.h"
#include "show_post.h"

struct timespec tm;

char display_name[40];
char created_at[40];
char content[800];
int  replies_count;
int  reblogs_count;
int  favorites_count;

void main(void)
{  
  while(1) // Run continuously.
    {
      tm.tv_nsec = 0;
      tm.tv_sec = 0; 
      clock_settime(CLOCK_REALTIME,&tm);
      
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

      while (tm.tv_sec < 60)
	clock_gettime(CLOCK_REALTIME,&tm);
    }
}
