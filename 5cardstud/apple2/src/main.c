#include <apple2.h>
#include <conio.h>
#include "hires.h"
#include "background.h"

unsigned i;

void main(void)
{

  hires_Init();
  background();
  cgetc();
  hires_Done();
}

