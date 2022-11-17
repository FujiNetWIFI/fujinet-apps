/**
 * SimpleTerm for ADAM
 */

#include <conio.h>
#include <stdio.h>
#include <eos.h>
#include <msx.h>

#define NET 0x0e

struct
{
  char cmd;
  unsigned short port;
  char name[256]; 
} host;

unsigned short port=23;
DCB *dcb=NULL;

char r[1024];

char k;

void find_dcb(void)
{
  dcb=eos_find_dcb(NET);
  if (dcb==NULL)
    {
      cprintf("Could not find FujiNet. Halting.");
      while (1) {}
    }
}

void banner(void)
{
#ifdef TEXT40
  msx_set_mode(0);
  clrscr();
  printf("\n");
#endif /* _TEXT40_ */
  printf("SIMPLETERM\n----------\n");
}

void connect(void)
{
  char ports[6]={0,0,0,0,0,0};
  
  while (host.name[0]==0x00)
    {
      cprintf("HOSTNAME: ");
      cgets(host.name);
      cprintf("PORT (23 BY DEFAULT): ");
      cgets(ports);
      host.port = atoi(ports);
    }

  // Attempt connect
  host.cmd = 0xFF; // Connect command.
  eos_write_character_device(NET,host,sizeof(host));
}

void in(void)
{
  k=eos_end_read_keyboard();

  if (k>1)
    {
      char out[2]={0xFE,0x00};
      out[1]=k;
      eos_write_character_device(NET,&out,sizeof(out));
      k=eos_start_read_keyboard();
    }
}

void out(void)
{
  char s = eos_read_character_device(NET,r,sizeof(r));
  if (s == 0x80)
    for (int i=0;i<dcb->len;i++)
      putchar(r[i]);
}

void term(void)
{
  k=eos_start_read_keyboard();

  while (1)
    {
      in();
      out();
    }
}

void main(void)
{
  find_dcb();
  banner();
  connect();
  term();
}
