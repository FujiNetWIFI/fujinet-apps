#include <apple2.h>
#include <conio.h>
#include "hires.h"
#include "background.h"
#include "charset.h"
#include "text.h"
#include "pot.h"

int i;

void main(void)
{
  hires_Init();
  background();
  status("HI ERIC IT WORKS YEAH\nSECOND LINE");

  pot(100);
  
  cgetc();
  hires_Done();
}

