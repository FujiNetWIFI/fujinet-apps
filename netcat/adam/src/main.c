/**
 * Network Function Testbed
 */
#include <conio.h>
#include <msx.h>
#include <eos.h>
#include "network.h"

unsigned char buf[1024];
unsigned char txbuf[64];
unsigned char t=0;

void in(void)
{
  unsigned short l = network_read(buf, sizeof(buf));
  
  if (l > 0)
    for (unsigned short i=0;i<l;i++)
      {
	cputc(buf[i]);
      }
}

void out(void)
{
  char k=0;
  
  k=eos_end_read_keyboard();

  if (k>1)
    {
      txbuf[0]=k;
      network_write(txbuf,1); // send one char.
      eos_start_read_keyboard();
    }
}

void main(void)
{
  char url[256];

  clrscr();
  
  cprintf("Welcome to netcat, an example #FujiNet Network Program\nThis is a very simple 80 column terminal program.\n\nAvailable protocols: TCP UDP TELNET HTTP/S TNFS FTP\n\nURL's are of the form:\n\nN:PROTO://host.name:port/path?key=val&key=val\n\nExamples of URL's:\n------------------\nN:TELNET://BBS.FOZZTEXX.NET:23/\nN:HTTPS://www.gnu.org/licenses/gpl-3.0.txt\n\nENTER URL: ");
  cgets(url);

  cprintf("OPEN  RETURN: %02X\n",network_open(url, 4, 0));

  eos_start_read_keyboard();
  
  while(1)
    {
      in();
      out();
    }
}
