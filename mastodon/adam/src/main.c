/**
 * mastodon - a simple mastodon client
 *
 * @author Thom Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#include <video/tms99x8.h>
#include <eos.h>
#include <smartkeys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "input.h"

#define CHANNEL_MODE_JSON 1
#define NET_DEV 0x09

const char *url = "N:HTTPS://oldbytes.space/api/v1/timelines/public?limit=1";

char response[1024];

struct _oc
{
  unsigned char cmd;
  char mode;
  char trans;
  char url[256];
} OC; // Open command

struct _scm
{
  unsigned char cmd;
  char mode;
} SCM; // Set Channel Mode command

struct _qc
{
  unsigned char cmd;
  char query[128];
} QC; // Query command

void main(void)
{
  unsigned char r;
  DCB *dcb;
  
  OC.cmd = 'O';
  OC.mode = 12;
  OC.trans = 3;
  strncpy(OC.url,url,256);

  SCM.cmd = 0xFC;
  SCM.mode = CHANNEL_MODE_JSON;

  QC.cmd = 'Q';
  
  eos_start_read_keyboard();
  
  smartkeys_set_mode();
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,"  NEXT\n  TOOT");
  smartkeys_status("  REALLY SIMPLE MASTODON CLIENT\n  WANNA MAKE A BETTER ONE?\n  YOU CAN!");

  while (1)
    {
    reload:
      vdp_vfill(MODE2_ATTR,0xF4,0x100);
      vdp_vfill(MODE2_ATTR+0x100,0xF5,0x100);
      vdp_vfill(MODE2_ATTR+0x200,0x1F,0x1200);

      vdp_vfill(0x0000,0x00,0x1400);

      while (eos_write_character_device(NET_DEV,(unsigned char *)OC,sizeof(OC)) < 0x80);

      while (eos_write_character_device(NET_DEV,(unsigned char *)SCM,sizeof(SCM)) < 0x80);
      while (eos_write_character_device(NET_DEV,"P",1) < 0x80);

      while (eos_request_status(NET_DEV,dcb) != 0x80);
      
      strncpy(QC.query,"/0/account/display_name",128);
      while (eos_write_character_device(NET_DEV,(unsigned char *)QC,sizeof(QC)) < 0x80);
      memset(response,0,1024);
      while (eos_read_character_device(NET_DEV,response,1024) < 0x80);
      vdp_color(15,4,7); gotoxy(0,0); cprintf("%s",response);

      strncpy(QC.query,"/0/created_at",128);
      while (eos_write_character_device(NET_DEV,(unsigned char *)QC,sizeof(QC)) < 0x80);
      memset(response,0,1024);
      while (eos_read_character_device(NET_DEV,response,1024) < 0x80);
      vdp_color(15,5,7); gotoxy(0,1); cprintf("%s",response);

      strncpy(QC.query,"/0/content",128);
      while (eos_write_character_device(NET_DEV,(unsigned char *)QC,sizeof(QC)) < 0x80);
      memset(response,0,1024);
      while (eos_read_character_device(NET_DEV,response,1024) < 0x80);
      vdp_color(1,15,7); smartkeys_puts(0,16,response);

      eos_write_character_device(NET_DEV,"C",1);
      
      while (1)
	{
	  switch(input())
	    {
	    case KEY_SMART_VI:
	      goto reload;
	    }
	}
    }
}
