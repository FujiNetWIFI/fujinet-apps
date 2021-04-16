/**
 * ADM-3A Terminal State Machine
 */

#include <atari.h>
#include "conio.h"
#include "term.h"

enum {
      CHAR,
      ESCAPE,
      SET_MODE,
      SET_ROW,
      SET_COL
} mode=CHAR;

bool term(char* buf, unsigned short len)
{
  char c;
  char ca[8]={0,0,0,0,0,0,0,0};
  unsigned short oldcol;
  unsigned char oldrow;
  
  do
    {
      switch(mode)
	{
	case CHAR:
	  switch(c=*buf++)
	    {
	    case 0x07: // BEL
	      c=0xFD;  // BUZZER
	      break;
	    case 0x08: // BS
	      c=0x7E;  // ATASCII BS
	      break;
	    case 0x0A: // LF
	      c=0;
	      oldcol=OS.colcrs;
	      ca[0]=0x9B; // EOL
	      print(ca);
	      OS.colcrs=oldcol;
	      break;
	    case 0x0B: // VT (upline)
	      if (OS.colcrs>0)
		c=0x1C; // UP ARROW
	      else
		c=0x00;
	      break;
	    case 0x0C: // FF ->
	      c=0x1F;
	      break;
	    case 0x0D: // CR
	      c=0;
	      OS.colcrs=0;
	      break;
	    case 0x1A: // Clear screen
	      c=0x7D;  // ATASCII CLS
	      break;
	    case 0x1B: // ESCAPE
	      mode=ESCAPE;
	      c=0;
	      break;
	    case 0x1E: // Home cursor
	      c=0;
	      OS.rowcrs=OS.colcrs=0;
	      break;
	    }
	  break;
	case ESCAPE:
	  switch(c=*buf++)
	    {
	    case '=': // Absolute position
	      c=0;
	      mode=SET_COL;
	      break;
	    case 'G': // Set mode
	      c=0;
	      mode=SET_MODE;
	      break;
	    default:
	      c=0;
	      mode=CHAR; // Go back to character mode.
	    }
	  break;
	case SET_MODE:
	  break;
	case SET_ROW:
	  c=*buf++;
	  if ((c>0x19) && (c<0x38))
	    {
	      OS.rowcrs=c-0x20;
	      c=0;
	    }
	  mode=CHAR;
	  break;
	case SET_COL:
	  c=*buf++;
	  if ((c>0x19) && (c<0x70))
	    {
	      OS.colcrs=c-0x20;
	      c=0;
	    }
	  mode=SET_ROW;
	  break;
	}

      if (c>0x00)
	{
	  printc(&c);
	}

      len--;
      
    } while (len>0);
    
  return false;
}
