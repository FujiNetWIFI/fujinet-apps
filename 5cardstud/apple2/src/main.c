#include <apple2.h>
#include <conio.h>
#include "hires.h"

unsigned i;

void main(void)
{

  hires_Init();
   
   for (i=0;i<39;++i) {
    hires_Mask(i,0,1,192,0xa92A);
    hires_Mask(++i,0,1,192,0xa955);
  }

  cgetc();
  hires_Done();
}

