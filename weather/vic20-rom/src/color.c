/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose color funcs
 */

#include <vic20.h>
#include <cbm.h>
#include <string.h>

void color_setup(void)
{
  VIC.volume_color=0x90; // aux = yellow
  VIC.bg_border_color=0x69;

  // Go ahead and pre-paint the color on screen
  memset(COLOR_RAM,0x01,506);

  cbm_k_bsout('\x05'); // white  
}
