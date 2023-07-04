#include <apple2.h>
#include <conio.h>
#include "hires.h"
#include "background.h"
#include "cards.h"

unsigned i;

void main(void)
{
  hires_Init();
  background();
  hires_Draw(0,0,1,8,ROP_CPY,&cards_bin[0]);
  cgetc();
  hires_Done();
}

