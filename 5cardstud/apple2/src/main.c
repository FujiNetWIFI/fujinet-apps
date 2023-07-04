#include <apple2.h>
#include <conio.h>
#include "hires.h"
#include "background.h"
#include "charset.h"

unsigned i;

void main(void)
{
  hires_Init();
  background();
  hires_Draw(0,0,1,8,ROP_OR(0x2A),&charset[0x208]);
  cgetc();
  hires_Done();
}

