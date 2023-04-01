/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Splash screen
 */

#include <conio.h>
#include "util.h"
#include "state.h"

extern MainState mainState;

static const char *l1="#FUJINET Network Utilities\0";
static const char *l2="Distributed under the GPL v. 3.0\0";

/**
 * @brief The Splash screen
 */
void splash(void)
{
  unsigned long i=16384;
  
  clrscr();

  cputsxy(util_center(l1),10,l1);
  cputsxy(util_center(l2),12,l2);

  while (i--); // Wait.
  
  mainState=MAIN_MENU;
}
