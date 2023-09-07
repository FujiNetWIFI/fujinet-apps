/**
 * @brief simple sprite tool
 * @author thomas cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details
 */

#include <stdlib.h>
#include <smartkeys.h>
#include <eos.h>
#include <msx.h>
#include <conio.h>
#include "input.h"
#include "cursor.h"

#define SPACEBAR 0x20
#define UP 0xA0
#define RIGHT 0xA1
#define DOWN 0xA2
#define LEFT 0xA3

#define SPRITE_X 64
#define SPRITE_Y 128

#define NET_DEV 0x09
#define READWRITE 0x0C

enum _state
  {
    SETUP,
    UPDATE_SPRITE,
    COMMAND,
    SEND_TO_NETWORK_GET_URL,
    SEND_TO_NETWORK_OPEN,
    SEND_TO_NETWORK_WRITE,
    SEND_TO_NETWORK_CLOSE
  } state;

enum _currentColor
  {
    TRANSPARENT = 0,
    BLACK,
    MEDIUM_GREEN,
    LIGHT_GREEN,
    DARK_BLUE,
    LIGHT_BLUE,
    DARK_RED,
    CYAN,
    MEDIUM_RED,
    LIGHT_RED,
    DARK_YELLOW,
    LIGHT_YELLOW,
    DARK_GREEN,
    MAGENTA,
    GRAY,
    WHITE
  } currentColor;

enum _currentColor current_color=BLACK;

unsigned char cursor_x=0;
unsigned char cursor_y=0;

unsigned char r1 = 0xE0;

unsigned char sprite_buf[8]={0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55};

unsigned char url[256];

void color_palette(void)
{
  for (unsigned char i=0;i<16;i++)
    {
      gotoxy(28,i);
      msx_color(1,7,7);
      cprintf("%2u ",i);
      msx_color(1,i,7);
      cputs(" ");
    }
}

void update_sprite(void)
{
  // Update the VDP's version of the sprite
  msx_vpoke(0x1b04,SPRITE_Y);
  msx_vpoke(0x1b05,SPRITE_X);
  msx_vpoke(0x1b06,0x01);
  msx_vpoke(0x1b07,current_color);
  
  msx_vwrite(sprite_buf, 0x3808, 8);
  
  // Plot the sprite
  gotoxy(0,0);
  
  for (unsigned char i=0;i<8;i++)
    {
      unsigned char b=sprite_buf[i];
      
      for (unsigned char j=0;j<8;j++)
	{
	  if (b & 0x80)
	    msx_color(1,current_color,7);
	  else
	    msx_color(1,(enum _currentColor)TRANSPARENT,7);

	  putch('.');
	  b <<= 1;
	}

      putch('\n');
    }
  
  // go ahead and update editor cursor here, too.
  cursor(true);
  cursor_pos(cursor_x,cursor_y);
  
  state=COMMAND;
}

void up(void)
{
  cursor_y--;

  if (cursor_y==0)
    cursor_y=8;
}

void right(void)
{
  cursor_x++;
  
  if (cursor_x>7)
    cursor_x=0;
}

void down(void)
{
  cursor_y++;

  if (cursor_y>7)
    cursor_y=0;
}

void left(void)
{
  cursor_x--;

  if (cursor_x==0xFF) // Because cursor_x is unsigned, it will wrap to 0xFF.
    cursor_x=7;
}

void toggle_bit(void)
{
  unsigned char b = sprite_buf[cursor_y];
  unsigned char bit=0;
  
  switch (cursor_x)
    {
    case 0:
      bit=0x80;
      break;
    case 1:
      bit=0x40;
      break;
    case 2:
      bit=0x20;
      break;
    case 3:
      bit=0x10;
      break;
    case 4:
      bit=0x08;
      break;
    case 5:
      bit=0x04;
      break;
    case 6:
      bit=0x02;
      break;
    case 7:
      bit=0x01;
      break;
    }

  sprite_buf[cursor_y] ^= bit;
}

void color(void)
{
  current_color++;
  current_color &= 0x0F;  
}

void mag(void)
{
  r1 ^= 0x01; // flip the mag bit in register 1

  eos_write_vdp_register(1,r1); // and write it back to VDP
}

void send_to_network_get_url(void)
{
  bool keyVI = false;
  
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,"  ABORT");
  smartkeys_status("   ENTER NETWORK URL, e.g.\n   N:TCP://1.2.3.4:1234/");

  input_line(0,19,0,url,255,&keyVI);

  if (keyVI)
    {
      state=SETUP;
      return;
    }
  else
    state=SEND_TO_NETWORK_OPEN;
}

void send_to_network_open(void)
{
  char open_cmd[259];

  open_cmd[0]='O';
  open_cmd[1]=READWRITE;
  open_cmd[2]=0;

  strncpy(&open_cmd[3],url,256);
  eos_write_character_device(NET_DEV,open_cmd,sizeof(open_cmd));
  state=SEND_TO_NETWORK_WRITE;
}

void send_to_network_close(void)
{
  char close_cmd='C';

  eos_write_character_device(NET_DEV,&close_cmd,1);
  state=SETUP;
}

void send_to_network_write(void)
{
  char write_cmd[9] = {'W',0,0,0,0,0,0,0,0};

  memcpy(&write_cmd[1],sprite_buf,sizeof(sprite_buf));

  eos_write_character_device(NET_DEV,write_cmd,sizeof(write_cmd));
  state=SEND_TO_NETWORK_CLOSE;
}

void command(void)
{
  unsigned char k=eos_read_keyboard();

  switch(k)
    {
    case SPACEBAR:
      toggle_bit();
      break;
    case SMARTKEY_IV:
      color();
      break;
    case SMARTKEY_V:
      mag();
      break;
    case SMARTKEY_VI:
      state=SEND_TO_NETWORK_GET_URL;
      return;
    case UP:
      up();
      break;
    case RIGHT:
      right();
      break;
    case DOWN:
      down();
      break;
    case LEFT:
      left();
      break;
    }
  
  state=UPDATE_SPRITE;
}

void setup(void)
{
  smartkeys_set_mode();
  color_palette();
  smartkeys_display(NULL,NULL,NULL,"  COLOR","   MAG"," SEND TO\n NETWORK");
  smartkeys_status("  SPRITE TOOL 0.1");
  state=UPDATE_SPRITE;
}

void main(void)
{
  while(true)
    {
      switch(state)
	{
	case SETUP:
	  setup();
	  break;
	case UPDATE_SPRITE:
	  update_sprite();
	  break;
	case COMMAND:
	  command();
	  break;
	case SEND_TO_NETWORK_GET_URL:
	  send_to_network_get_url();
	  break;
	case SEND_TO_NETWORK_OPEN:
	  send_to_network_open();
	  break;
	case SEND_TO_NETWORK_WRITE:
	  send_to_network_write();
	  break;
	case SEND_TO_NETWORK_CLOSE:
	  send_to_network_close();
	  break;
	}
    }
}

