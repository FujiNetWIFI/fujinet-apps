/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include "system.h"
#include "bar.h"

static unsigned char bar_y, bar_c, bar_m, bar_i;

void bar_init(unsigned char y, unsigned char c, unsigned char m, unsigned char i)
{
  bar_y=y;
  bar_c=c;
  bar_m=m-1;
  bar_i=i;

  screen_bar(bar_y,bar_c,bar_m,bar_i,true);
}

void bar_up(void)
{
  if (bar_i > 0)
    {
      screen_bar(bar_y,bar_c,bar_m,bar_i,false);
      bar_i--;
      screen_bar(bar_y,bar_c,bar_m,bar_i,true);
    }
}

void bar_down(void)
{
  if (bar_i < bar_m)
    {
      screen_bar(bar_y,bar_c,bar_m,bar_i,false);
      bar_i++;
      screen_bar(bar_y,bar_c,bar_m,bar_i,true);
    }
}

char bar_get(void)
{
  return bar_i;
}

void bar_set(char i)
{
  screen_bar(bar_y,bar_c,bar_m,bar_i,false);
  bar_i=i;
  screen_bar(bar_y,bar_c,bar_m,bar_i,true);
}

void bar_done(void)
{
  screen_bar(bar_y,bar_c,bar_m,bar_i,false);
}

