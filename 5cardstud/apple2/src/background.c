/**
 * @brief fill background with greenness.
 */

#include <string.h>
#include <stdlib.h>
#include "hires.h"

void background(void)
{
  int i;
  for (i=0;i<39;++i) {
    hires_Mask(i,0,1,192,0xa92A);
    hires_Mask(++i,0,1,192,0xa955);
  }  
}
