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
  status("HI ERIC IT WORKS YEAH\nSECOND LINE");
  cgetc();
  hires_Done();
}

