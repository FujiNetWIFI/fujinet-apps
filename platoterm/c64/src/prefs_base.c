/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * prefs.c - Preferences menu functions
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <conio.h>
#include <tgi.h>
#include <string.h>
#include <serial.h>
#include <mouse.h>
#include <ctype.h>
#include "io.h"
#include "screen.h"
#include "keyboard.h"
#include "prefs.h"
#include "protocol.h"
#include "terminal.h"
#include "config.h"
#include "touch.h"
#ifdef __APPLE2ENH__
#include "./apple2enh/dhgr_subs.h"
#endif

extern ConfigInfo config;

uint8_t prefs_running;
static padBool TTYSave;
static padPt TTYLocSave;
static uint8_t CharWideSave;
static uint8_t CharHighSave;
static uint8_t CurMemSave;
static DispMode CurModeSave;
static padBool ModeBoldSave;
static padBool RotateSave;
static padBool ReverseSave;
extern padBool TTY;
extern padPt TTYLoc;
extern uint8_t CharWide;
extern uint8_t CharHigh;
extern DispMode CurMode;
extern padBool ModeBold;
extern padBool Rotate;
extern padBool Reverse;

extern uint8_t FONT_SIZE_Y;

uint8_t temp_val[8];
uint8_t ch;
uint8_t prefs_need_updating;
uint8_t touch_prefs_updated;
uint8_t io_prefs_updated;

/**
 * Run the preferences menu
 */
void prefs_run(void)
{
  keyboard_clear();
  TTYSave=TTY;
  TTYLocSave.x = TTYLoc.x;
  TTYLocSave.y = TTYLoc.y;
  CharWideSave=CharWide;
  CharHighSave=CharHigh;
  CharWide=8;
  CharHigh=16;
  CurMem=M0;
  CurModeSave=CurMode;
  ModeBoldSave=ModeBold;
  RotateSave=Rotate;
  ReverseSave=Reverse;
  TTY=true;
  prefs_running=true;
  touch_prefs_updated=false;
  io_prefs_updated=false;
  prefs_need_updating=false;
  prefs_clear();
  while (prefs_running)
    {
      prefs_serial();
    }

  if (prefs_need_updating)
    {
      prefs_update();
    }
  
  TTY=TTYSave;
  TTYLoc.x=TTYLocSave.x;
  TTYLoc.y=TTYLocSave.y;
  prefs_done();
}

/**
 * prefs_serial()
 * Preferences menu to show for serial devices.
 */
void prefs_serial(void)
{
  prefs_clear();  
  prefs_display("i)nterface d)river b)aud t)ouch o)ther s)ave e)xit: ");

  ch=prefs_get_key_matching("idbtoseIDBTOSE");

  switch(ch)
    {
    case 'i':
      prefs_select("interface");
      prefs_interface();
      break;
    case 'd':
      prefs_select("driver");
      prefs_driver();
      break;
    case 'b':
      prefs_select("baud");
      prefs_baud();
      break;
    case 't':
      prefs_select("touch");
      prefs_touch();
      break;
    case 'o':
      prefs_select("other");
      prefs_other();
      break;
    case 's':
      prefs_select("save");
      prefs_save();
      break;
    case 'e':
      prefs_running=false;
      break;
    }
  
}

/**
 * prefs_get_val()
 * get string with ip address numbers, terminated by return.
 */
void prefs_get_val(void)
{
  unsigned char strp=0;
  
  ch=0;

  while (ch != 0x0d)
    {
      ch=prefs_get_key_matching1("0123456789");
      if (ch==0x08) /* was translated from 0x14 to 0x08 */
  	{
	  if (strp>0)
	    {
	      --strp;
	      temp_val[strp]=0;
	      ShowPLATO(&ch,1);
	    }
  	}
      else if (ch==0x0d)
	{
	  // Don't append or show the CR
	}
      else
  	{
  	  temp_val[strp]=ch;
  	  ShowPLATO(&ch,1);
	  ++strp;
  	}
    }
}

/**
 * prefs_other()
 * Other prefs options
 */
void prefs_other(void)
{
  prefs_clear();
  prefs_display("OTHER: x)onoff e)xit: ");
  ch=prefs_get_key_matching("fxeFXE");

  switch(ch)
    {
    case 'x':
      prefs_xonoff();
      break;
    case 'e':
      prefs_serial();
      break;
    }
}

/**
 * prefs_xonoff()
 * Set xon/off buffers
 */
void prefs_xonoff(void)
{
  prefs_clear();
  prefs_display("enter new xon threshold: ");
  prefs_get_val();
  if (strcmp(temp_val,"")!=0)
    config.xon_threshold=atoi(temp_val);
  prefs_select(" ok");
  prefs_clear();
  prefs_display("enter new xoff threshold: ");
  prefs_get_val();
  if (strcmp(temp_val,"")!=0)
    config.xoff_threshold=atoi(temp_val);
  prefs_select(" ok");
  prefs_clear();
}


/**
 * prefs_save(void)
 * Save preferences
 */
void prefs_save(void)
{
  prefs_display("saving preferences...");
#ifdef __ATARI__
  ser_close();
#endif
  config_save();
  prefs_select("ok");
#ifdef __ATARI__
  io_open();
#endif
  prefs_clear();
}

/**
 * prefs_baud(void)
 * Preferences for selecting serial baud rate
 */
void prefs_baud(void)
{
  prefs_display("3)00 1)200 2)400 9)600 q)19200 w)38400 b)ack: ");
  
  ch=prefs_get_key_matching("3129qwbQWB");

  switch(ch)
    {
    case '3':
      prefs_select("300");
      config.baud=SER_BAUD_300;
      break;
    case '1':
      prefs_select("1200");
      config.baud=SER_BAUD_1200;
      break;
    case '2':
      prefs_select("2400");
      config.baud=SER_BAUD_2400;
      break;
    case '9':
      prefs_select("9600");
      config.baud=SER_BAUD_9600;
      break;
    case 'q':
      prefs_select("19200");
      config.baud=SER_BAUD_19200;
      break;
    case 'w':
      prefs_select("38400");
      config.baud=SER_BAUD_38400;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
  if (ch!='b')
    {
      io_prefs_updated=true;
      prefs_need_updating=true;
    }
}

/**
 * prefs_interface(void)
 * Preferences menu to select interface.
 */
void prefs_interface(void)
{
  prefs_clear();
  prefs_display("interface - e)thernet s)erial b)ack: ");

  ch=prefs_get_key_matching("sbSB");

  switch(ch)
    {
#if defined(__APPLE2__) || defined(__APPLE2ENH__)
    case 's':
      prefs_slot();
      break;    
#endif
    case 'b':
      prefs_select("back");
      break;
    }
}

#if defined(__APPLE2__) || defined(__APPLE2ENH__)
/**
 * prefs_slot(void)
 * Preferences menu to select card slot.
 */
void prefs_slot(void)
{
  prefs_clear();
  prefs_display("serial slot - 1) thru 7) b)ack: ");

  ch=prefs_get_key_matching("1234567bB");

  if (ch >= '1' && ch <= '7')
    {
      config.slot=ch - '0';
    }
  else if (ch == 'b')
    {
      prefs_select("back");
    }
}
#endif

/**
 * prefs_display(text)
 * Display a line of the preferences menu
 * This routine contains some ifdefs to work around the fact that the commodore targets
 * for CC65 remap ASCII passed in character strings.
 */
void prefs_display(const char* text)
{
  uint8_t c;
#if !defined(__C64__) || !defined(__C128__)
  unsigned char* capped_text;
#endif
  TTYLoc.x=0;
  TTYLoc.y=1;
  
  c=tgi_getcolor();
  tgi_setcolor(TGI_COLOR_WHITE);
#if !defined(__C64__) || !defined(__C128__)
  capped_text=strupr((unsigned char* )text);
  ShowPLATO((unsigned char*)capped_text, strlen(text));
#else
  ShowPLATO((unsigned char*)text, strlen(text));
#endif
  tgi_setcolor(c);
}

/**
 * Wait for a key matching input, return it.
 */
unsigned char prefs_get_key_matching(const char* matches)
{
  unsigned char ch;
  unsigned char i;
  
  while (true)
    {
      ch=tolower(cgetc());
      for (i=0;i<strlen(matches);++i)
	{
	  if (ch==matches[i])
	    return ch;
	}
    }
}

/**
 * TEMPORARY: Wait for a key matching input, return it.
 */
unsigned char prefs_get_key_matching1(const char* matches)
{
  unsigned char ch=0;
  unsigned char i;
  
  for (;;)
    {
      ch=cgetc();

      if (ch==0x0d || ch==0x9B)
	return 0x0d;
      else if (ch==0x14)
	return 0x08; /* convert PETSCII DEL to ASCII BS */
      
      for (i=0;i<strlen(matches);++i)
	{
	  if (ch==matches[i])
	    return ch;
	}
    }
}

/**
 * erase prefs bar
 */
void prefs_clear(void)
{
  uint8_t c;
#ifdef __APPLE2ENH__
  hue(DHGR_COLOR_BLACK);
  dhbar(0,185,519,191);
  hue(DHGR_COLOR_WHITE);
#else
  c=tgi_getcolor();
  tgi_setcolor(TGI_COLOR_BLACK);
  tgi_bar(0,actualSize.y-1-FONT_SIZE_Y,tgi_getmaxx(),actualSize.y-1);
  tgi_setcolor(c);
  ShowPLATO("\n\v",2);
#endif
}

/**
 * indicate selection, display it, and wait a bit for visual confirmation.
 */
void prefs_select(const char* text)
{
  unsigned char i=0;
  ShowPLATO((unsigned char *)text,strlen(text));
  
  for (i=0;i<100;i++)
    {
      screen_wait();
    }

  prefs_clear();

}

/**
 * Update program state with selected preferences
 */
void prefs_update(void)
{
  unsigned char retv;

  if (io_prefs_updated==true)
    {
      // Close any serial drivers.
      prefs_clear();
      prefs_display("closing serial driver...");
      ser_close();
      prefs_clear();
      prefs_display("unloading serial driver...");
#if defined(__C64__)
      // This is a workaround because I suspect up2400 incorrectly unloads itself.
      ser_uninstall();
#endif
      ser_unload();
      prefs_clear();
    }

  prefs_clear();
  
  if (touch_prefs_updated==true)
    {
      // Close any touch drivers
      prefs_display("unloading touch driver...");
      mouse_unload();
      prefs_clear();
    }

  if (io_prefs_updated==true)
    {
      prefs_display("loading serial driver...");
      ser_load_driver(io_ser_driver_name(config.driver_ser));
      
      io_init_funcptrs();
      io_open();
      prefs_clear();
    }  

  prefs_clear();
  
  if (touch_prefs_updated==true)
    {
      prefs_clear();
      prefs_display("loading touch driver...");
      retv = mouse_load_driver(&mouse_def_callbacks,touch_driver_name(config.driver_mou));
      if (retv==MOUSE_ERR_OK)
	{
	  prefs_select("ok");
	  mouse_show();
	}
      else
	{
	  prefs_select("error");
	}
    }
}

/**
 * prefs_check_for_touch_change
 */
void prefs_check_for_touch_change(void)
{
  if (ch!='b')
    {
      touch_prefs_updated=true;
      prefs_need_updating=true;
    }
}

/**
 * prefs_check_for_io_change
 */
void prefs_check_for_io_change(void)
{
  if (ch!='b')
    {
      io_prefs_updated=true;
      prefs_need_updating=true;
    }
}

/**
 * close prefs. 
 */
void prefs_done(void)
{
  TTY=TTYSave;
  CharWide=CharWideSave;
  CharHigh=CharHighSave;
  CurMem=CurMemSave;
  ModeBold=ModeBoldSave;
  Rotate=RotateSave;
  Reverse=ReverseSave;
  prefs_clear();
  TTYLoc.x=TTYLocSave.x;
  TTYLoc.y=TTYLocSave.y;
}
