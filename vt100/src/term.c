/**
 * VT100 Terminal State Machine
 */

#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include "conio.h"
#include "term.h"

enum {
      CHAR,
      ESCAPE,
      COMMAND
} mode=CHAR;

bool term(char* buf, unsigned short len)
{
  char c;                // Character to output to screen (0 = nothing)
  unsigned char p[16];   // 16 possible parameter values per esc sequence
  unsigned char* pp;     // Pointer to next parameter to set.
  char t[8];             // temp char array for parameter values
  char* tp;              // T pointer
  char or, oc;           // saved col/row
  
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
	    case 0x0B: // VT
	    case 0x0C: // FF
	      c=0;
	      if (OS.rowcrs<24)
		{
		  print("\x1D"); // Cursor down
		}
	      else // Don't move cursor, but scoot screen upward.
		{
		  OS.rowcrs=0;
		  print("\x9C"); // Delete line
		  OS.rowcrs=23;
		}
	      break;
	    case 0x0D: // CR
	      c=0;
	      OS.colcrs=0;
	      break;
	    case 0x1B: // ESCAPE
	      mode=ESCAPE;
	      c=0;
	      break;
	    }
	  break;
	case ESCAPE:
	  switch(c=*buf++)
	    {
	    case '[':
	      mode=COMMAND;
	      tp=&t[0]; // Beginning of tmp parameter
	      pp=&p[0]; // Beginning of parameters
	      memset(t,0,sizeof(t)); // clear temp
	      c=0;
	      break;
	    }
	  break;
	case COMMAND:
	  c=*buf++;
	  if ((c>'/') && (c<':'))
	    {
	      // Numeric value, concatenate to tmp
	      *tp=c;
	      tp++;
	      c=0;
	    }
	  else if (c==';') // Next parameter
	    {
	      *pp=atoi(t);           // convert from tmp to param value
	      pp++;                  // increment parameter value
	      memset(t,0,sizeof(t)); // Clear tmp
	      c=0;
	    }
	  else // Command
	    {
	      *pp=atoi(t);           // convert from tmp to param value
	      memset(t,0,sizeof(t)); // clear tmp

	      switch(c)              // c contains letter command
		{
		case 'A':            // Cursor UP (stop at top)
		  do
		    {
		      if (OS.rowcrs>0)
			c=0x1C;          // ATASCII UP
		      else
			c=0x00;
		      if (p[0]>0)
			p[0]--;
		    } while (p[0]>0); 
		      break;
		case 'B':            // Cursor DOWN (stop at bottom)
		  do
		    {
		      if (OS.rowcrs<23)
			c=0x1D;
		      else
			c=0x00;
		      if (p[0]>0)
			p[0]--;
		    } while (p[0]>0);
		  break;
		case 'C':            // Cursor RIGHT (stop at right)
		  do
		    {
		      if (OS.colcrs<79)
			c=0x1F;
		      else
			c=0x00;
		      if (p[0]>0)
			p[0]--;
		    } while (p[0]>0);
		  break;
		case 'D':            // Cursor LEFT (stop at left)
		  do
		    {
		      if (OS.colcrs>0)
			c=0x1E;
		      else
			c=0x00;
		      if (p[0]>0)
			p[0]--;
		    } while (p[0]>0);
		  break;
		case 'H':            // Cursor position
		  if ((p[0]==0) && (p[1]==0))
		    OS.rowcrs=OS.colcrs=0;
		  else
		    {
		      OS.rowcrs=p[0]-1;
		      OS.colcrs=p[1]-1;
		    }
		  break;
		case 'J':            // Clear
		  c=0;               // consume char
		  switch(p[0])       // 1st parameter gives clear type
		    {
		    case 0:          // clear from current pos to bottom 
		      break;
		    case 1:          // clear from top to current pos
		      break;
		    case 2:          // CLEAR WHOLE SCREEN
		      or=OS.rowcrs;
		      oc=OS.colcrs;
		      print("\x7D"); // ATASCII CLS char
		      OS.rowcrs=or;
		      OS.colcrs=oc;
		      break;
		    }
		  break;
		case 'm':            // Mode
		  break;
		}
	      
	      mode=CHAR;             // Back to char mode.
	    }
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
