/**
 * Generate scale table for ISS
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details
 */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  if (argc<3)
    {
      printf("%s <xres> <yres>\n",argv[0]);
      exit(1);
    }

  float x = atof(argv[1]) / 360.0;
  float y = atof(argv[2]) / 180.0;  

  printf("unsigned char xpos[360] = \n");
  printf("  { ");
  
  for (float i=-180.0; i<180.0; i++)
    {
      printf("%d,",(unsigned char)floor((x*i) + (atof(argv[1]) / 2)));
    }

  printf("  };\n\nunsigned char ypos[360] = \n  { ");
  
    
    for (float i=90.0; i>-90.0; i--)
    {
      printf("%d,",(unsigned char)floor((y*i) + (atof(argv[2]) / 2)));
    }

  printf("  };\n");
  
  return 0;}
