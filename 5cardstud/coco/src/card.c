/**
 * @brief card display routines
 */

#include "card.h"
#include "text.h"
#include "hires.h"

/**
 * @brief display a single card at x,y
 * @param x Horizontal position (0-39)
 * @param y Vertical position (0-191)
 * @param c Card to display
 * @param s Suit to display
 */
void card(unsigned char x, unsigned char y, Card c, Suit s)
{
  hires_putc(x,y,ROP_XOR(0),0x09);
  hires_putc(x+1,y,ROP_XOR(0),0x0A);
  
  // Card body
  /* hires_Mask(x,y,2,24,ROP_WHITE); */

  
  
}
