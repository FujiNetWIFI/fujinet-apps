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

#include <cmoc.h>
#include "vt100.h"
#include "screen.h"

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
    OSC,
    PRIVATE,
    CHARSET,
    HASH,
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
 * @brief Non-zero while parsing a DEC private-mode sequence (ESC [ ? ...).
 */
static char _private=0;

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
extern void set_region(unsigned char top, unsigned char bottom);
extern void ind(void);
extern void ri(void);
extern void nel(void);
extern void whereami(unsigned char *r, unsigned char *c);
extern void save_cursor(void);
extern void restore_cursor(void);
extern void decaln(void);
extern void set_autowrap(unsigned char on);
extern void set_origin(unsigned char on);
extern void set_reverse(unsigned char on);
extern void set_appcursor(unsigned char on);

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
extern void fg_color(unsigned char c);
extern void bg_color(unsigned char c);
extern void fg_default(void);
extern void bg_default(void);

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
  _private=0;
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
	case 39:
	  fg_default();
	  break;
	case 49:
	  bg_default();
	  break;
	default:
	  if (_pv[i] >= 30 && _pv[i] <= 37)
	    fg_color(_pv[i] - 30);
	  else if (_pv[i] >= 90 && _pv[i] <= 97)
	    fg_color(_pv[i] - 90);
	  else if (_pv[i] >= 40 && _pv[i] <= 47)
	    bg_color(_pv[i] - 40);
	  else if (_pv[i] >= 100 && _pv[i] <= 107)
	    bg_color(_pv[i] - 100);
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
  char num[6];

  switch (_pv[0])
    {
    case 5: // ARE WE OK? Yes, always ok.
      sendback('\x1b');
      sendback('[');
      sendback('0');
      sendback('n');
      break;
    case 6: // Where are we on screen?
      whereami(&r,&c);

      /* CPR reply, built with itoa10 to avoid pulling in printf/sprintf
         (which cost >1KB and overflowed the tight $4000-$7FFF stack space).
         Row/col are reported 1-based per the VT100 spec (whereami is 0-based). */
      sendback('\x1b');
      sendback('[');
      itoa10(r+1,num);
      for (i=0;num[i];i++) sendback(num[i]);
      sendback(';');
      itoa10(c+1,num);
      for (i=0;num[i];i++) sendback(num[i]);
      sendback('R');
      break;
    }
}

/**
 * @brief Device Attributes (DA): reply identifying us as a VT100 with the
 *        Advanced Video Option (we have attributes/colour). vttest queries this.
 */
void da(void)
{
  static const char r[] = "\x1b[?1;2c";
  unsigned char i;
  for (i=0;r[i];i++)
    sendback(r[i]);
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
    case 'f': /* HORIZ & VERTICAL POSITION (same as H) */
      cup(_pv[0],_pv[1]);
      break;
    case 'r': /* SET SCROLL REGION (DECSTBM) */
      set_region(_pv[0],_pv[1]);
      break;
    case 'c': /* DEVICE ATTRIBUTES */
      da();
      break;
    case 's': /* SAVE CURSOR (ANSI) */
      save_cursor();
      break;
    case 'u': /* RESTORE CURSOR (ANSI) */
      restore_cursor();
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
  /* Accumulate this digit into the current parameter value numerically (not as
     a string + atoi), so any number of leading zeros parses correctly - e.g.
     ESC[00000000004;0000010H -> 4;10. The old 3-char string buffer truncated
     such sequences and sent everything to (1,1). */
  if (_pi < 10)
    _pv[_pi] = (char) (_pv[_pi] * 10 + (_c - '0'));

  vt100_state=BRACKET;
}

/**
 * @brief Consume an OSC/string sequence (ESC ] ...) until BEL or ST. Not
 *        implemented; swallowed so its payload doesn't leak as text.
 */
static void _vt100_osc(void)
{
  if (_c == BEL)
    vt100_state = CHAR;
  else if (_c == ESC)
    vt100_state = ESCAPE;       /* ST (ESC \): escape handler eats the '\' */
}

/**
 * @brief Consume a private-mode CSI sequence (ESC [ ? ...) to its final
 *        letter and ignore it (e.g. bracketed paste ESC[?2004h).
 */
static void _vt100_private(void)
{
  if (IS_COMMAND(_c))
    _vt100_cleanup();
}

/**
 * @brief Consume a character-set designator (ESC ( / ) / * / + <char>). We use
 *        one fixed CoCo font, so the designator is read and ignored - this
 *        stops sequences like ESC(B or ESC(0 from leaking their final byte.
 */
static void _vt100_charset(void)
{
  vt100_state = CHAR;
}

/**
 * @brief Process the byte after ESC # . ESC#8 is DECALN (screen test pattern);
 *        the line-size variants (#3..#6) we can't render, so ignore them.
 */
static void _vt100_hash(void)
{
  if (_c == '8')
    decaln();
  vt100_state = CHAR;
}

/**
 * @brief Apply a DEC private mode set (h) / reset (l): ESC [ ? Pn ... h|l.
 *        Only the modes we can honour are handled; the rest are ignored.
 */
static void _vt100_private_command(void)
{
  unsigned char i, set;

  if (_c == 'h' || _c == 'l')
    {
      set = (_c == 'h');
      for (i=0;i<_pi;i++)
	{
	  switch (_pv[i])
	    {
	    case 1:                    /* DECCKM: application cursor keys */
	      set_appcursor(set);
	      break;
	    case 3:                    /* DECCOLM: we can't change width on the */
	      clear_entire_screen();   /* CoCo, but the mode switch clears and  */
	      cup(1,1);                /* homes the cursor on a real VT100      */
	      break;
	    case 5:                    /* DECSCNM: reverse video */
	      set_reverse(set);
	      break;
	    case 6:                    /* DECOM: origin mode */
	      set_origin(set);
	      break;
	    case 7:                    /* DECAWM: autowrap */
	      set_autowrap(set);
	      break;
	    }
	}
    }

  _vt100_cleanup();
}

/**
 * @brief Called to process and determine PARAMETER or BRACKETCOMMAND state
 */
static void _vt100_bracket(void)
{
  if (_c == '?')
    {
      _private = 1;          /* DEC private mode: keep parsing the parameters */
      return;
    }
  if (_c == '>' || _c == '=')
    {
      vt100_state = PRIVATE; /* secondary DA / keypad: consume and ignore */
      return;
    }

  if (IS_PARAMETER(_c))
    {
      vt100_state=PARAMETER;
      _vt100_parameter();
    }
  else if (IS_PARAMETER_DELIMITER(_c))
    {
      if (_pi < 9)          /* advance to the next parameter slot */
        _pi++;
    }
  else if (IS_COMMAND(_c))
    {
      if (_pi < 10)         /* count the parameter we just accumulated */
        _pi++;

      if (_private)
        _vt100_private_command();
      else
        {
          vt100_state=BRACKETCOMMAND;
          _vt100_bracketcommand();
        }
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
  else if (_c == ']' || _c == 'P' || _c == '^' || _c == '_')
    vt100_state=OSC;            /* OSC/DCS/PM/APC string: consume and ignore */
  else if (_c == '\\')
    vt100_state=CHAR;           /* ST (ESC \) string terminator: just end it */
  else if (_c == '(' || _c == ')' || _c == '*' || _c == '+')
    vt100_state=CHARSET;        /* G0-G3 charset designation: consume designator */
  else if (_c == '#')
    vt100_state=HASH;           /* line-size / DECALN: handle next byte */
  else if (_c == '7')
    { save_cursor(); vt100_state=CHAR; }        /* DECSC */
  else if (_c == '8')
    { restore_cursor(); vt100_state=CHAR; }     /* DECRC */
  else if (_c == 'c')
    {                                           /* RIS - reset to initial state */
      set_reverse(0);
      set_appcursor(0);
      reset_attributes();
      set_region(0,0);
      set_autowrap(1);
      set_origin(0);
      clear_entire_screen();
      cup(1,1);
      vt100_state=CHAR;
    }
  else if (IS_COMMAND(_c))
    {
      /* ESC-letter commands (IND/RI/NEL/HTS) are a single byte after ESC, so
         act on it now - deferring to COMMAND state would run the command
         against the *next* byte and swallow it (broke vi's RI scroll-up). */
      vt100_state=COMMAND;
      _vt100_command();
    }
  else
    vt100_state=CHAR;
}

/**
 * @brief Called when processing non-escape characters
 */
static void _vt100_char(void)
{
  /* CoCo fast path: printable characters are the common case, so handle them
     first and go straight to the screen, skipping the control-code compare
     chain and the toscreen() indirection. Same rule as toscreen() (c > 0x1F). */
  if (_c > 0x1F)
    {
      screen_putc(_c);
      return;
    }

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
  /* CoCo: no per-character bank switching or cursor toggling - the cursor is
     drawn in screen_flush - so this runs straight into the state machine. */
  _c = __c;

  /* C0 control characters are acted on immediately, even in the middle of a
     CSI/escape sequence, WITHOUT aborting it - real VT100 behavior, and what
     vttest's "cursor-control characters inside ESC sequences" checks (e.g.
     ESC[2<BS>C). ESC restarts a sequence; CAN/SUB abort it. CHAR/OSC/CHARSET/
     HASH consume their own bytes, so this only applies while parsing a CSI. */
  if ((unsigned char) _c < 0x20 &&
      vt100_state != CHAR && vt100_state != OSC &&
      vt100_state != CHARSET && vt100_state != HASH)
    {
      if (_c == ESC)
        { vt100_state = ESCAPE; return; }
      if (_c == CAN || _c == SUB)
        { _vt100_cleanup(); return; }
      {
        enum vt100_states resume = vt100_state;
        vt100_state = CHAR;
        _vt100_char();              /* run BS/CR/LF/HT/VT/... */
        vt100_state = resume;       /* then resume the sequence */
      }
      return;
    }

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
    case OSC:
      _vt100_osc();
      break;
    case PRIVATE:
      _vt100_private();
      break;
    case CHARSET:
      _vt100_charset();
      break;
    case HASH:
      _vt100_hash();
      break;
    }
}

/* CoCo: true when the decoder is in its normal CHAR state (not parsing an
   escape sequence), so the input layer can safely bulk-write printable runs. */
unsigned char vt100_char_state(void)
{
  return vt100_state == CHAR;
}

/* Reset the decoder to a clean CHAR state (e.g. after an abrupt disconnect,
   so a following clear sequence isn't swallowed by a half-parsed escape). */
void vt100_reset(void)
{
  _vt100_cleanup();
}
