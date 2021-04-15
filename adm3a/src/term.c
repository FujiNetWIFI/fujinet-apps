/**
 * Terminal implementation for ADM3A
 */

#include <stdbool.h>
#include <atari.h>
#include "conio.h"
#include "term.h"

static unsigned short oldcol=0;
static unsigned char ormask=0;
static unsigned char andmask=0xFF;

static enum {
	     CHAR,
	     ESC,
	     CURSOR_COL,
	     CURSOR_ROW,
	     SET_MODE
} termMode;

bool term(char* buf, unsigned short len)
{
  while (len > 0)
    {
      char c=0;
      
      switch (termMode)
	{
	case CHAR:
	  switch(*buf++)
	    {
	    case 0x07: // BEL
	      c=0xFD; // ATASCII BUZZ
	      break;
	    case 0x08: // BS
	      c=0x7E; // ATASCII DELETE
	      break;
	    case 0x0a: // LF
	      oldcol = OS.colcrs; // get current horiz position.
	      c=0x9B; // Change to ATASCII EOL.
	      OS.colcrs = oldcol; // restore horiz position.
	      break;
	    case 0x0b: // VT
	      if (OS.rowcrs == 0)
		c=0x9D; // INSERT LINE
	      else
		c=0x1C; // UP position.
	      break;
	    case 0x0c: // FF ->
	      c=0x1E;
	      break;
	    case 0x0d: // CR
	      OS.colcrs=0;
	      break;
	    case 0x0e: // SO, IGNORE
	    case 0x0f: // SI, IGNORE
	      break;
	    case 0x1A: // Clear screen
	      c=0x7D;
	      break;
	    case 0x1B: // Escape Mode
	      termMode=ESC;
	      break;
	    case 0x1E: // Home cursor
	      OS.colcrs=0;
	      OS.rowcrs=0;
	      break;
	    default:
	      c=*buf;
	      break;
	    }
	  break;
	case ESC:
	  switch (*buf++)
	    {
	    case 'G': // Set mode
	      termMode=SET_MODE;
	      break;
	    case '=': // Cursor position
	      termMode = CURSOR_COL;
	      break;
	    default:
	      termMode = CHAR;
	      break;
	    }
	  break;
	case CURSOR_COL:
	  if ((*buf > 0x1f) && (*buf < 0x70))
	    OS.colcrs = *buf-0x20;
	  *buf++;
	  termMode=CURSOR_ROW;
	  break;
	case CURSOR_ROW:
	  if ((*buf > 0x1f) && (*buf < 0x38))
	    OS.rowcrs = *buf-0x20;
	  *buf++;
	  termMode=CHAR;
	  break;
	case SET_MODE:
	  switch (*buf++)
	    {
	    case '0': // NORMAL TEXT
	    case '2': // BLINKING TEXT
	    case '8': // UNDERLINE
	    case ':': // UNDERLINE BLINKING
	    case '@': // REDUCE
	    case 'B': // REDUCE BLINKING
	    case 'H': // REDUCE UNDERLINE
	    case 'J': // REDUCE UNDERLINE BLINKING
	      ormask = 0;
	      andmask = 0xFF;
	      break;
	    case '1': // BLANK TEXT
	    case 'P': // GRAPHICS NORMAL VIDEO
	    case 'Q': // GRAPHICS BLANK
	    case 'R': // GRAPHICS BLINKING
	    case 'T': // GRAPHICS REVERSE
	    case 'V': // GRAPHICS BLINKING REVERSE
	    case 'X': // GRAPHICS UNDERLINE
	    case 'Z': // GRAPHICS UNDERLINE BLINKING
	    case '\\': // GRAPHICS UNDERLINE REVERSE
	    case '^': // GRAHPICS UNDERLINE REVERSE BLINKING
	    case '`': // GRAPHICS REDUCE
	    case 'b': // GRAPHICS REDUCE BLINKING
	    case 'd': // GRAPHICS REDUCE REVERSE
	    case 'f': // GRAPHICS REDUCE REVERSE BLINKING
	    case 'h': // GRAPHICS REDUCE UNDERLINE
	    case 'j': // GRAPHICS REDUCE UNDERLINE BLINKING
	    case 'l': // GRAPHICS REDUCE UNDERLINE REVERSE
	    case 'n': // GRAPHICS REDUCE UNDERLINE REVERSE BLINKING
	      ormask = 0x00;
	      andmask = 0x00;
	      break;
	    case '4': // REVERSE TEXT
	    case '6': // REVERSE BLINKING
	    case '<': // REVERSE UNDERLINE
	    case '>': // REVERSE UNDERLINE BLINKING
	    case 'D': // REVERSE REDUCE
	    case 'F': // REVERSE REDUCE BLINKING
	    case 'L': // REVERSE REDUCE UNDERLINE
	    case 'N': // REVERSE REDUCE UNDERLINE BLINKING
	      ormask = 0x80;
	      andmask = 0xFF;
	      break;	      
	    }
	}

      // Print character if not NULL.
      if (c!=0)
	{
	  c=(c & andmask) | ormask;
	  printc(&c);
	}

      // do this until we're done.
      len--;
    }
    
  return false;
}
