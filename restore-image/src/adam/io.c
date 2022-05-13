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
#include <conio.h>
#include "io.h"
#include "die.h"

#define FUJI_DEV 0x0F
#define NET_DEV 0x09
#define DISK_DEV 0x07

#define RESPONSE_SIZE 1024
#define ACK 0x80
#define READ_HOST_LIST "\xF4"
#define FILE_MAXLEN 36

extern char hs[8][32];
extern unsigned char selected_host_slot;
extern unsigned char selected_tape;

static char response[1024];
static DCB *dcb;

static void io_command_and_response(void* buf, unsigned short len)
{
  memset(response,0,sizeof(response));
  eos_write_character_device(FUJI_DEV,buf,len);
  eos_read_character_device(FUJI_DEV,response,sizeof(response));
}

void io_init(void)
{
  io_perform_close();
  eos_start_read_keyboard(); // might as well. :)
  dcb=eos_find_dcb(NET_DEV);
}

void io_select_host(void)
{
  io_command_and_response("\xF4",1);
  memcpy(hs,response,256);
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

void io_perform_open(char *path)
{
  struct {
    unsigned char hostSlot;
    unsigned char mode;
    unsigned char file[FILE_MAXLEN];
  } deviceSlots[8];

  struct
  {
    char cmd;
    char ds;
    char mode;
  } mi;

  struct
  {
    char cmd;
    char ds;
    char fn[256];
  } sdf;

  struct
  {
    char cmd;
    char deviceSlots[304];
  } pds;

  memset(deviceSlots,0,sizeof(deviceSlots));
  memset(mi,0,sizeof(mi));
  memset(sdf,0,sizeof(sdf));
  memset(pds,0,sizeof(pds));
  
  mi.cmd = 0xF8;  /* Mount Image in Device Slot */
  mi.ds  = 0x03;  /* Use device slot 4 */
  mi.mode = 0x00; /* Read only */
  
  sdf.cmd = 0xE2; /* Set Device Slot Filename */
  sdf.ds  = 0x03; /* Use device slot 4 */
  strcpy(sdf.fn,path);

  /* Read; fill in device slot; put back. */
  io_command_and_response("\xF2",1); /* GET DEVICE SLOTS */
  memcpy(deviceSlots,response,304);

  deviceSlots[3].hostSlot = selected_host_slot;
  deviceSlots[3].mode     = 0x00; /* READ ONLY */
  strncpy(deviceSlots[3].file,path,FILE_MAXLEN);

  pds.cmd = 0xF1; /* Put Device Slots cmd */
  memcpy(pds.deviceSlots,deviceSlots,304);

  eos_write_character_device(FUJI_DEV,pds,sizeof(pds));

  /* Finally set the device filename for the slot... */
  eos_write_character_device(FUJI_DEV,sdf,sizeof(sdf));

  /* ...and mount it. */
  eos_write_character_device(FUJI_DEV,mi,sizeof(mi));
}

void io_perform_close(void)
{
  struct
  {
    char cmd;
    char ds;
  } ui;

  ui.cmd = 0xE9; /* unmount disk image */
  ui.ds  = 0x03; /* use device slot 4  */

  eos_write_character_device(FUJI_DEV,ui,sizeof(ui));
}

bool io_perform_read_block(unsigned long blockNum, unsigned char *buf)
{
  return eos_read_block(DISK_DEV,blockNum,buf) == 0x80;
}

unsigned char io_perform_write_error(void)
{
  DCB *dcb;

  eos_request_device_status(selected_tape,dcb);
  return eos_get_device_status(NET_DEV);
}

unsigned char io_perform_write_block(unsigned char dev, unsigned long blockNum, unsigned char *buf)
{
  return eos_write_block(dev,blockNum,buf) == 0x00;
}

unsigned char io_perform_verify_block(unsigned char dev, unsigned long blockNum, unsigned char *buf)
{
  return eos_read_block(dev,blockNum,buf) == 0x80;  
}

bool io_perform_eom(void)
{
  DCB *dcb;
  char s;
  
  eos_request_device_status(DISK_DEV,dcb);
  s=eos_get_device_status(DISK_DEV) & 0x0F;

  return s == 0x02; /* Missing block? */
}

void io_perform_rewind(void)
{
  DCB *dcb;

  dcb=eos_find_dcb(selected_tape);

  dcb->block=0;
  dcb->status=2;
}

#endif /* BUILD_ADAM */
