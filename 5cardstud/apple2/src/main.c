#include <apple2.h>
#include <conio.h>
#include "hires.h"

void main(void)
{
  hires_Init();
  hires_Mask(0,0,559,191,ROP_CONST(0x2A));
  cgetc();
  hires_Done();
}
