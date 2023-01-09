/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * terminal_char_load.c - Character set loading routine for 8x6 font.
 */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "../terminal.h"
#include "../protocol.h"

// Temporary PLATO character data, 8x16 matrix
static unsigned char char_data[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static unsigned char BTAB[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01}; // flip one bit on (OR)

static unsigned char curr_word;	    // current word
static unsigned char u;		    // loop counter

extern unsigned char fontm23[768];

/**
 * terminal_char_load - Store a character into the user definable
 * character set.
 */
void terminal_char_load(padWord charNum, charData theChar)
{
  // Clear char data. 
  memset(char_data,0,sizeof(char_data));

  // load and transpose character data into 8x16 array
  for (curr_word=0; curr_word<8; curr_word++)
    {
      for (u=16; u-->0; )
	{
	  if (theChar[curr_word] & 1<<u)
	    {
	      char_data[u^0x0F&0x0F]|=BTAB[curr_word];
	    }
	}
    }

  // OR pixel rows together from 16 to 6 rows
  fontm23[(charNum*6)+0]=char_data[0]|char_data[1]|char_data[2];
  fontm23[(charNum*6)+1]=char_data[3]|char_data[4];
  fontm23[(charNum*6)+2]=char_data[5]|char_data[6]|char_data[7];
  fontm23[(charNum*6)+3]=char_data[8]|char_data[9];
  fontm23[(charNum*6)+4]=char_data[10]|char_data[11]|char_data[12];
  fontm23[(charNum*6)+5]=char_data[13]|char_data[14]|char_data[15];
}
