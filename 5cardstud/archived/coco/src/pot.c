/**
 * @brief Pot display
 */

#include <cmoc.h>
#include "hires.h"
#include "text.h"

/**
 * @brief Display pot
 * @param n The number to display in pot
 */
void pot(unsigned short n)
{
  char s[4]={0,0,0,0};

  // Convert pot to text
  itoa(n,s,10);
  
  // Draw the border
  hires_putc(17,96,ROP_AND(0x55),';');
  hires_putc(18,96,ROP_AND(0x2A),'@');
  hires_putc(19,96,ROP_AND(0x55),'@');
  hires_putc(20,96,ROP_AND(0x2A),'@');
  hires_putc(21,96,ROP_AND(0x55),'@');
  hires_putc(22,96,ROP_AND(0x2A),'<');
  hires_putc(17,104,ROP_AND(0x55),'?');
  hires_putc(22,104,ROP_AND(0x2A),'?');
  hires_putc(17,112,ROP_AND(0x55),'=');
  hires_putc(18,112,ROP_AND(0x2A),'@');
  hires_putc(19,112,ROP_AND(0x55),'@');
  hires_putc(20,112,ROP_AND(0x2A),'@');
  hires_putc(21,112,ROP_AND(0x55),'@');
  hires_putc(22,112,ROP_AND(0x2A),'>');

  // Draw pot
  hires_putc(18,104,ROP_CPY,'o');
  hires_putc(19,104,ROP_OR(0x55),s[0]);
  hires_putc(20,104,ROP_OR(0x2A),s[1]);
  hires_putc(21,104,ROP_OR(0x55),s[2]);
}

