/**
 * @brief Netcat, a terminal emulator for FujiNet
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose main()
 */

#include <stdbool.h>
#include "globals.h"
#include "init.h"

void main(void)
{
  while (true)
    {
      switch(state)
	{
	case INIT:
	  init();
	  break;
	}
    }
}
