/**
 * @brief quick little tool to get board offsets as table
 */
#include <stdio.h>
#include "../src/pfdata.h"

void main(void)
{
  printf("unsigned char _board_pos[32] = {");

  for (unsigned char i=20;i<180;i++)
    if (pfdata[i])
      printf("%u,",i);

  printf("};\n");
}
