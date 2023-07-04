#include <apple2.h>
#include <conio.h>
#include "hires.h"
#include "background.h"
#include "charset.h"

int i;

void main(void)
{
  hires_Init();
  background();

  for (i='A';i<'I';i++)
    {
      int o = i << 3;
      hires_Draw(i-'A',0,1,8,ROP_OR(0x2A),&charset[o]);
    }
  
  cgetc();
  hires_Done();
}

