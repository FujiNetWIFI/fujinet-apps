/**
 * Tweet
 */

#include <string.h>
#include "tweet.h"
#include "screen.h"

/* const char tweets[2][] = */
/*   {"Anyone used cyclomethicone for lubricating old floppy disks?\x9b" */
/* "I was recommend to try it for recovering dying disks, but I'm kinda worried about it because all the places I can find that'll sell it are health & beauty places, so I'm worried I'll end up with lavender scented disks"}, */
/*   {"If we can afford to give $740 billion to the Pentagon, please don’t tell me we can’t afford to eliminate homelessness in America and cancel the rent during a pandemic. Instead of more bombs and more missiles, we need more jobs, more rent relief and more affordable housing."}; */

/* const char tweet_users[2][] = {"foone <@foone>", */
/* 			    "Bernie Sanders <@SenSanders>"}; */

char p11[]={64,65,66,67};
char p12[]={68,69,70,71};
char p13[]={72,73,74,75};
char p14[]={76,77,78,79};

extern unsigned char *video_ptr;

void tweets(void)
{
  screen_puts(5,2,"Bernie Sanders \xA0\xB3\xE5\xEE\xB3\xE1\xEE\xE4\xE5\xF2\xF3 35m");
  screen_puts(5,4,"If we can afford to give $740");
  screen_puts(5,5,"billion to the Pentagon, please");
  screen_puts(0,6,"don't tell me we can't afford to eliminate homelessness in America and cancel the rent during a pandemic. Instead of more bombs and more missiles, we need more jobs, more rent relief and more affordable housing.");
  screen_puts(0,2,"\x10\x11\x12\x13");
  screen_puts(0,3,"\x14\x15\x16\x17");
  screen_puts(0,4,"\x18\x19\x1a\x1b");
  screen_puts(0,5,"\x1c\x1d\x1e\x1f");
  screen_puts(0,12,"  \x7d 407     \x60 3.6K     \x7b 15.5K     \x7c");

  screen_puts(5,13,"foone \xA0\xE6\xEF\xEF\xEE\xE5 4h");
  screen_puts(5,14,"VCR heads have to spin (unlike");
  screen_puts(5,15,"cassette tape heads) because");
  screen_puts(0,16,"they're using helical-scan recording to fit more data onto the tape, so they use a wireless slip ring to take power and data into the head, and to bring data back out.");

  memcpy(&video_ptr[520],&p11,sizeof(p11));
  memcpy(&video_ptr[560],&p12,sizeof(p12));
  memcpy(&video_ptr[600],&p13,sizeof(p13));
  memcpy(&video_ptr[640],&p14,sizeof(p14));
  
  screen_puts(0,23,"  \x7d 3       \x60          \x7b 57        \x7c");

  
}
