/**
 * @brief fill background with greenness.
 */

#include <string.h>
#include <stdlib.h>
#include "hires.h"
#include "text.h"

#define BOTTOM 167

void background(void)
{
  int i;
  for (i=0;i<39;++i) {
    hires_Mask(i,0,1,192,0xa92A);
    hires_Mask(++i,0,1,192,0xa955);
  }  

  /* Place border */
  hires_putc(0,0,ROP_AND(0x2A),0x01);
  hires_putc(39,0,ROP_AND(0x55),0x02);
  hires_putc(0,BOTTOM,ROP_AND(0x2A),0x03);
  hires_putc(39,BOTTOM,ROP_AND(0x55),0x04);
}
