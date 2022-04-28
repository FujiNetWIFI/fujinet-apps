/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifdef BUILD_ADAM

#include <eos.h>
#include <string.h>
#include "io.h"
#include "die.h"

#define FUJI_DEV 0x0F
#define RESPONSE_SIZE 1024
#define ACK 0x80
#define READ_HOST_LIST "\xF4"

extern HostSlots hs[8];

static char response[1024];

static void io_command_and_response(void* buf, unsigned short len)
{
  memset(response,0,sizeof(response));
  eos_write_character_device(FUJI_DEV,buf,len);
  eos_read_character_device(FUJI_DEV,response,sizeof(response));
}

void io_init(void)
{
  eos_start_read_keyboard(); // might as well. :)
}

void io_select_host(void)
{
  unsigned char r = eos_write_character_device(FUJI_DEV,READ_HOST_LIST,1);

  if (r != ACK)
    die("COULD NOT SEND HOST LIST CMD");

  r = eos_read_character_device(FUJI_DEV,&hs,RESPONSE_SIZE);

  if (r != ACK)
    die("COULD NOT READ HOST LIST");  
}

void io_close_directory(void)
{
  eos_write_character_device(FUJI_DEV,"\xF5",1);
}

void io_mount_host_slot(unsigned char hs)
{
  unsigned char c[2]={0xF9,0x00};

  c[1] = hs;

  eos_write_character_device(FUJI_DEV,&c,sizeof(c));
}

void io_open_directory(unsigned char h, char *p, char *f)
{
  char c[258]={0xF7};
  char *e;
  
  c[1] = h;
  strcpy(&c[2],p);
  e=&c[2];

  if (f[0]!=0x00)
    {
      while (*e != 0x00)
	e++;

      e++;
      strcpy(e,f);
    }

  eos_write_character_device(FUJI_DEV,&c,sizeof(c));
}

void io_set_directory_position(DirectoryPosition pos)
{
  unsigned char c[3]={0xE4,0x00,0x00};

  memcpy(&c[1],&pos,sizeof(DirectoryPosition));

  eos_write_character_device(FUJI_DEV,&c,sizeof(c));
}

char *io_read_directory(unsigned char l, unsigned char a)
{
  unsigned char c[3]={0xF6,0x00,0x00};
  c[1]=l;
  c[2]=a;
  io_command_and_response(&c,3);
  return response;
}

#endif /* BUILD_ADAM */
