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

  hires_putc(17,96,ROP_AND(0x55),';');
  hires_putc(18,96,ROP_AND(0x2A),'@');
  hires_putc(19,96,ROP_AND(0x55),'@');
  hires_putc(20,96,ROP_AND(0x2A),'@');
  hires_putc(21,96,ROP_AND(0x55),'@');
  hires_putc(22,96,ROP_AND(0x2A),'<');
  hires_putc(17,104,ROP_AND(0x55),'?');
  hires_putc(22,104,ROP_AND(0x2A),'?');
  hires_putc(17,112,ROP_AND(0x55),'=');
  hires_putc(18,112,ROP_AND(0x2A),'@');
  hires_putc(19,112,ROP_AND(0x55),'@');
  hires_putc(20,112,ROP_AND(0x2A),'@');
  hires_putc(21,112,ROP_AND(0x55),'@');
  hires_putc(22,112,ROP_AND(0x2A),'>');

  hires_putc(19,104,ROP_OR(0x55),'1');
  hires_putc(20,104,ROP_OR(0x2A),'0');
  hires_putc(21,104,ROP_OR(0x55),'0');
  
  cgetc();
  hires_Done();
}

