/**
 * @brief fill background with greenness.
 */

#include <string.h>
#include <stdlib.h>
#include "hires.h"

void hires_Fill(void)
{
  int i;
  
  for (i=0;i<20;i++)
    {
      hires_Mask(i*2,0,1,191,0xA92A);
      hires_Mask(i*2+1,0,1,191,0xA955);
    }
}
