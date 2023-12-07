/**
 * Weather 
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#include <conio.h>
#include "init.h"
#include "welcome.h"
#include "options.h"
#include "location.h"
#include "weather.h"
#include "constants.h"
#include "forecast.h"

State state=LOCATION;

void main(void)
{
  init();
  welcome();
  options();
  
  while (1)
  {
    switch(state)
      {
      case LOCATION:
	location();
	break;
      case WEATHER:
	weather();
	break;
      case FORECAST:
	forecast();
	break;
      case DONE:
	break;
      }
  }
}
