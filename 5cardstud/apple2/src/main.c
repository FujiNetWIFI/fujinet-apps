#include <apple2.h>
#include <tgi.h>
#include <conio.h>

void main(void)
{
  tgi_install(tgi_static_stddrv);
  tgi_init();
  tgi_clear();
  tgi_setpixel(100,100);
  cgetc();
  tgi_done();
}
