#include <apple2.h>
#include <conio.h>
#include "hires.h"

void main(void)
{
  hires_Init();
  hires_Fill();
  cgetc();
  hires_Done();
}
