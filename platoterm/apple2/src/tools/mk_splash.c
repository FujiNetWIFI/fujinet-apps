#include <stdio.h>
#include "../apple2/splash.c"

void main(void)
{
  FILE *fp;
  fp=fopen("splash.bin","w");
  fwrite(splash,1,sizeof(splash),fp);
  fclose(fp);
}
