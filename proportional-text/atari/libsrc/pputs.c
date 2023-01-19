/**
 * Proportional Font text output library for Atari
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#define NULL 0

/** 
 * @var _pcx Cursor X position (pcursor.c)
 */
extern unsigned short _pcx;

/** 
 * @var _pcx Cursor Y position (pcursor.c)
 */
extern unsigned char _pcy;

extern unsigned char pputc(const char c);

/**
 * @brief Write string to screen using proportional font, at cursor.
 * @param c NULL terminated string to display
 */
unsigned char pputs(const char *c)
{
  unsigned char oldx=_pcx;
  unsigned char w;
  
  // map ASCII to appropriate character
  while (*c != NULL)
    {      
      if (*c == '\n')
	{
	  _pcx=oldx;
	  _pcy += 8;
	  c++;
	  continue;
	}

      w = pputc(*c);

      if ((_pcx + w) < 319)
	{
	  _pcx += w; // Move X right by width pixels
	  c++; // advance to next char;
	}
      else
	{
	  _pcx = oldx;
	  _pcy += 8;
	  c++;
	}
    }
  
  return w;
}
