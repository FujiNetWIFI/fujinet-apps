/**
 * FujiNet Configuration Program
 *
 * Screen functions
 */

#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "screen.h"

unsigned char* video_ptr;
unsigned char* dlist_ptr;
unsigned short screen_memory;
unsigned char* font_ptr;

void screen_clear()
{
  memset(video_ptr,0x00,GRAPHICS_0_SCREEN_SIZE);
}

void screen_clear_line(unsigned char y)
{
  memset(&video_ptr[(y)*20],0,20);
}

/**
 * Print ATASCII string to display memory
 */
void screen_puts(unsigned char x,unsigned char y,char *s)
{
  char offset;
  
  do
    {     
      if (*s < 32)
	{
	  offset=64;
	}
      else if (*s<96)
	{
	  offset=-32;
	}
      else
	{
	  offset=0;
	}
      
      SetChar(x++,y,*s+offset);

      ++s;
      
    } while(*s!=0);
}
