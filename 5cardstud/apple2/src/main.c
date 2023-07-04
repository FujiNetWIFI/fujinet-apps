#include <apple2.h>
#include <conio.h>
#include "hires.h"
#include "background.h"
#include "charset.h"
#include "text.h"
#include "pot.h"
#include "card.h"

int i;

void main(void)
{
  unsigned char x,y;
  
  hires_Init();
  background();
  status("HI ERIC IT WORKS YEAH\nSECOND LINE");

  pot(100);

  // /////////////////////////////////////////////
  
  card(18,140,CARD_2,SUIT_SPADE);

  // /////////////////////////////////////////////
  
  cgetc();
  hires_Done();
}

