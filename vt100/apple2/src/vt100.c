/**
 * @brief VT100 Protocol Decoder
 * 
 * @verbose A lean decoder for DEC VT100 Terminal sequences.
 *          To implement, compile in, and implement callback functions.
 *          To use, call vt100(c); for each character
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vt100.h"
#include "screen.h" // temporary!

/**
 * @brief the current terminal state
 */
static enum vt100_states
  {
    CHAR,
    ESCAPE,
    COMMAND,
    BRACKET,
    PARAMETER,
    BRACKETCOMMAND,
    CLEANUP,
  } vt100_state=CHAR;

/**
 * @brief The current character being processed
 */
static char _c;

/**
 * @brief The current parameter index (0-10)
 */
static char _pi=0;

/**
 * @brief Parameter values array [0-9]
 */
static char _pv[10];

/**
  * @brief The current parameter string (one digit at a time), 3 digits max.
  */
static char _ps[4];

/**
 * @brief is char c a number?
 */
#define IS_PARAMETER(_c) (_c > '/') && (_c < ':')

/**
 * @brief is char the parameter delimiter ';' ? 
 */
#define IS_PARAMETER_DELIMITER(_c) _c == ';'

/**
 * @brief is char c a command?
 */
#define IS_COMMAND(_c) (_c > '@') && (_c < '{')

/*********************************************************************/

/* Hooks before/after doing terminal processing */
extern void use(void);
extern void unuse(void);

/* Hook to toggle cursor */
extern void cursor_toggle(void);

/* response injection */
extern void sendback(char c);

/* Character commands */
extern void bel(void);
extern void bs(void);
extern void tab(void);
extern void lf(void);
extern void cr(void);
extern void xoff(void);
extern void xon(void);
extern void toscreen(char c);

/* Cursor control */
extern void cuu(void);
extern void cud(void);
extern void cur(void);
extern void cul(void);
extern void cup(unsigned char r, unsigned char c);
extern void cupr(unsigned char r);
extern void ind(void);
extern void ri(void);
extern void nel(void);
extern void whereami(unsigned char *r, unsigned char *c);

/* insert/delete */
extern void insert_line(unsigned char n);
extern void delete_line(unsigned char n);

/* Tab control */
extern void hts(void);
extern void clear_current_tab_stop(void);
extern void clear_all_tab_stops(void);

/* Character attributes */
extern void reset_attributes(void);
extern void bold(void);
extern void lo(void);
extern void underline(void);
extern void blink(void);
extern void inverse(void);
extern void invisible(void);

/* Clear commands */
extern void clear_cursor_to_end(void);
extern void clear_beginning_to_cursor(void);
extern void clear_entire_screen(void);
extern void clear_to_end_of_line(void);
extern void clear_line_until_cursor(void);
extern void clear_entire_line(void);

/* Media Control commands */
extern void printer_off(void);
extern void printer_on(void);

/*********************************************************************/

/**
 * @brief called to clean up parameters and reset parameter index
 */
static void _vt100_cleanup(void)
{
  _pi=
    _ps[0]=
    _pv[0]=
    _pv[1]=
    _pv[2]=
    _pv[3]=
    _pv[4]=
    _pv[5]=
    _pv[6]=
    _pv[7]=
    _pv[8]=
    _pv[9]=0;
  vt100_state=CHAR;
}

/**
 * @brief wrap cursor commands to handle repeat
 */
void _vt100_bracketcommand_cursor(void)
{
  unsigned char i;
  unsigned char r = _pv[0];

  if (r==0)
    r=1;
  
  for (i=0;i<r;i++)
    {
      switch(_c)
	{
	case 'A': /* CURSOR UP */
	  cuu();
	  break;
	case 'B': /* CURSOR DN */
	  cud();
	  break;
	case 'C': /* CURSOR RT */
	  cur();
	  break;
	case 'D': /* CURSOR LF */
	  cul();
	  break;
	}
    }
}

/**
 * @brief insert line
 */
void il(void)
{
  if (_pv[0]==0)
    _pv[0]=1;

  insert_line(_pv[0]);
}

/**
 * @brief Set Character Attributes
 */
void sgr(void)
{
  unsigned char i;

  for (i=0;i<_pi;i++)
    {
      switch(_pv[i])
	{
	case 0:
	  reset_attributes();
	  break;
	case 1:
	  bold();
	  break;
	case 2:
	  lo();
	  break;
	case 4:
	  underline();
	  break;
	case 5:
	  blink();
	  break;
	case 7:
	  inverse();
	  break;
	case 8:
	  invisible();
	  break;
	}
    }
}

/**
 * @brief Erase in Display
 */
void ed(void)
{
  switch(_pv[0])
    {
    case 0: /* CURSOR TO END */
      clear_cursor_to_end();
      break;
    case 1: /* BEGINNING TO CURSOR */
      clear_beginning_to_cursor();
      break;
    case 2: /* ENTIRE SCREEN */
      clear_entire_screen();
      break;
    }
}

/**
 * @brief Erase in line
 */
void el(void)
{
  switch(_pv[0])
    {
    case 0:
      clear_to_end_of_line();
      break;
    case 1:
      clear_line_until_cursor();
      break;
    case 2:
      clear_entire_line();
      break;
    }
}

/**
 * @brief Media control (printer on/off)
 */
void mc(void)
{
  switch (_pv[0])
    {
    case 4:
      printer_off();
      break;
    case 5:
      printer_on();
      break;
    }
}

/**
 * @brief Tab Clear control
 */
void tbc(void)
{
  switch (_pv[0])
    {
    case 0:
      clear_current_tab_stop();
      break;
    case 3:
      clear_all_tab_stops();
      break;
    }
}

/**
 * @brief Device Status Report
 */
void dsr(void)
{
  unsigned char r,c,i;
  char report[8];
  
  switch (_pv[0])
    {
    case 5: // ARE WE OK? Yes, always ok.
      sendback('\x1b');
      sendback('[');
      sendback('0');
      sendback('n');
    case 6: // Where are we on screen?
      whereami(&r,&c);

      sprintf(report,"\x1b[%u;%uR",r,c);

      for (i=0;i<strlen(report);i++)
	sendback(report[i]);
      break;
    }
}

/**
 * @brief called to process non-bracket command
 */
static void _vt100_command(void)
{
  switch(_c)
    {
    case 'D':
      ind();
      break;
    case 'M':
      ri();
      break;
    case 'E':
      nel();
      break;
    case 'H':
      hts();
      break;
    }
  
  _vt100_cleanup();
}

/**
 * @brief called to process bracket command
 */
static void _vt100_bracketcommand(void)
{
  char tmp[5] = {0,0,0,0,0};
  
  switch(_c)
    {
    case 'A': /* CURSOR UP */
    case 'B': /* CURSOR DN */
    case 'C': /* CURSOR RT */
    case 'D': /* CURSOR LT */
      _vt100_bracketcommand_cursor();
      break;
    case 'H': /* CURSOR POS */
      cup(_pv[0],_pv[1]);
      break;
    case 'f': /* CURSOR POS HORIZ */
      cupr(_pv[0]);
      break;
    case 'm': /* SET ATTRIBUTES */
      sgr();
      break;
    case 'J': /* ERASE IN DISPLAY */
      ed();
      break;
    case 'K': /* ERASE IN LINE */
      el();
      break;
    case 'L': /* INSERT LINE(s) */
      il();
      break;
    case 'M': /* DELETE LINE(s)*/
      delete_line(_pv[0]);
      break;
    case 'g': /* TAB CLEAR */
      tbc();
      break;
    case 'i': /* PRINTER CONTROL */
      mc();
      break;
    case 'n': /* DEVICE STATUS REPORT */
      dsr();
      break;
    }
  
  _vt100_cleanup();
}

/**
 * @brief called to process next byte of parameter
 */
static void _vt100_parameter(void)
{
  char cs[2];
  
  cs[0]=_c;
  cs[1]=0;
  
  if (strlen(_ps) < 4)
    strcat(_ps,cs);

  vt100_state=BRACKET;
}

/**
 * @brief Called to process and determine PARAMETER or BRACKETCOMMAND state
 */
static void _vt100_bracket(void)
{
  if (IS_PARAMETER(_c))
    {
      vt100_state=PARAMETER;
      _vt100_parameter();
    }
  else if (IS_PARAMETER_DELIMITER(_c))
    {
      _pv[_pi] = atoi(_ps); /* Store finished parameter value */

      memset(_ps,0,sizeof(_ps));
      _pi++;                 /* Finally increment to next parameter */
    }
  else if (IS_COMMAND(_c))
    {
      _pv[_pi] = atoi(_ps); /* Store finished parameter value */

      memset(_ps,0,sizeof(_ps));
      _pi++;                 /* Finally increment to next parameter */

      vt100_state=BRACKETCOMMAND;
      _vt100_bracketcommand();
    }
  else
    {
      _vt100_cleanup();
    }
}

/**
 * @brief Called when processing the ESCAPE character
 */
static void _vt100_escape(void)
{
  if (_c == '[')
    vt100_state=BRACKET;
  else if (IS_COMMAND(_c))
    vt100_state=COMMAND;
  else
    vt100_state=CHAR;  
}

/**
 * @brief Called when processing non-escape characters
 */
static void _vt100_char(void)
{
  switch (_c)
    {
    case BEL:
      bel();
      break;
    case BS:
      bs();
      break;
    case TAB:
      tab();
      break;
    case VT:
    case FF:
    case LF:
      lf();
      break;
    case CR:
      cr();
      break;
    case DC1:
      xoff();
      break;
    case DC3:
      xon();
      break;
    case ESC:
      vt100_state=ESCAPE;
      break;
    default:
      toscreen(_c);
      break;
    }
}

/**
 * @brief the terminal state machine
 * @param __c The next character to process
 */
void vt100(char __c)
{
  use();
  cursor_toggle();
  _c = __c;

  switch(vt100_state)
    {
    case CHAR:
      _vt100_char();
      break;
    case ESCAPE:
      _vt100_escape();
      break;
    case COMMAND:
      _vt100_command();
      break;
    case BRACKET:
      _vt100_bracket();
      break;
    case PARAMETER:
      _vt100_parameter();
      break;
    case BRACKETCOMMAND:
      _vt100_bracketcommand();
      break;
    case CLEANUP:
      _vt100_cleanup();
      break;
    }
  unuse();
  cursor_toggle();
}
