/**
 * vt100 terminal emulator
 */

#include "vt100.h"

/**
 * Process NUL character
 */
void state0_NUL(void)
{
}

/**
 * Process BEL character
 */
void state0_BEL(void)
{
}

/**
 * Process BS character
 */
void state0_BS(void)
{
}

/**
 * Process HT character
 */
void state0_HT(void)
{
}

/**
 * Process LF character
 */
void state0_LF(void)
{
}

/**
 * Process VT character
 */
void state0_VT(void)
{
}

/**
 * Process FF character
 */
void state0_FF(void)
{
}

/**
 * Process CR character
 */
void state0_CR(void)
{
}

/**
 * Process CAN character
 */
void state0_CAN(void)
{
}

/**
 * Process BS character
 */
void state0_SUB(void)
{
}

/**
 * Process ESC character
 */
void state0_ESC(void)
{
}

/**
 * Process any other state 0 character
 */
void state0_default(void)
{
}

/**
 * State 0 codes 
 */
const char state0_codes[]=
  { 0x00, // NUL
    0x07, // BEL
    0x08, // BS (backspace)
    0x09, // HT (horizontal tab)
    0x0A, // LF (Line Feed)
    0x0B, // VT (Vertical tab)
    0x0C, // FF (form feed, aka clear)
    0x0D, // CR (carriage return)
    0x18, // CAN (Cancel, demote to state 0)
    0x1A, // SUB (Cancel, demote to state 0)
    0x1B, // ESC (Escape, promote to state 1)
  };

/**
 * State 0 function pointers
 */
const void (*state0_func[11])() =
  {
   state0_NUL,
   state0_BEL,
   state0_BS,
   state0_HT,
   state0_LF,
   state0_VT,
   state0_FF,
   state0_CR,
   state0_CAN,
   state0_SUB,
   state0_ESC
  };

/**
 * Parse VT100 State 0 characters
 * @param c - pointer to current character
 */
void vt100_state0_parse(char* c)
{
  unsigned char i=0;

  for (i=0;i<sizeof(state0_codes);i++)
    {
      if (*c == state0_codes[i])
	{
	  state0_func[i]();
	  return;
	}
    }

  // Standard character
  state0_default();
  
}

/**
 * main()
 */
void main(void)
{
  
}
