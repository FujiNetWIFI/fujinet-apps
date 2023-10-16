/**
 * @brief Print out PLATO keys on printer
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details
 */

#include <video/tms99x8.h>
#include <conio.h>
#include <eos.h>
#include "printout.h"

void main(void)
{
  while(1)
    {
      vdp_set_mode(1);
      vdp_set_border(VDP_INK_DARK_BLUE);
      vdp_vfill(0x2000,0xF4,32);
      clrscr();
      
      cprintf("--------------------------------");
      cprintf("     PRINT PLATOTERM KEYS       ");
      cprintf("--------------------------------");
      cprintf("                                ");
      cprintf("         PRESS ANY KEY          ");
      cprintf("                                ");
      cprintf("      TO SEND TO PRINTER        ");
      cgetc();
      cprintf("                                ");
      cprintf("          PRINTING...           ");
      
      for (int i=0;i<sizeof(printout_txt);i++)
	eos_print_character(printout_txt[i]); 
    }
}
