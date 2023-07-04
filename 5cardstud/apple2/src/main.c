#include <apple2.h>
#include <conio.h>
#include "hires.h"
#include "background.h"
#include "charset.h"
#include "text.h"

int i;

void main(void)
{
  hires_Init();
  background();

  hires_puts(0,0,ROP_INV,"HELLO WORLD");
  
  cgetc();
  hires_Done();
}

