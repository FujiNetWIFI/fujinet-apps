/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * touch.c - Touchscreen functions
 */

#include <stdbool.h>
#include <string.h>
#include <mouse.h>
#include <peekpoke.h>
#include "touch.h"

#ifdef __ATARI__
#include <atari.h>
#endif

static padBool TouchActive;

static struct mouse_info mouse_data;
static uint16_t screen_w;
static uint16_t screen_h;
static uint8_t mouse_present=false;
static uint8_t mou_res=0;

extern uint16_t scaletx[];
extern uint16_t scalety[];

/**
 * touch_init() - Set up touch screen
 */
void touch_init(void)
{
}

/**
 * touch_allow - Set whether touchpanel is active or not.
 */
void touch_allow(padBool allow)
{
#ifndef __APPLE2__
  /* // If mouse is off screen (due to previously being moved off screen, move onscreen to make visible. */
  /* if (allow) */
  /*   { */
  /*     mouse_move(previous_mouse_x,previous_mouse_y); */
  /*   } */
  /* else */
  /*   { */
  /*     if (mouse_data.pos.x != screen_w && mouse_data.pos.y != screen_h) */
  /* 	{ */
  /* 	  previous_mouse_x = mouse_data.pos.x; */
  /* 	  previous_mouse_y = mouse_data.pos.y; */
  /* 	  mouse_move(screen_w,screen_h); */
  /* 	} */
  /*   } */
  TouchActive=allow;
#endif
}

/**
 * touch_main - Process mouse events and turn into scaled touch events
 */
void touch_main(void)
{
#ifndef __APPLE2__
  uint8_t lastbuttons;
  padPt coord;

  if (mouse_present==false)
    return;
  
  mouse_info(&mouse_data);
  
  if (mouse_data.buttons == lastbuttons)
    return; /* debounce */
  else if ((mouse_data.buttons & MOUSE_BTN_LEFT))
    {
      touch_translate(&coord);
      Touch(&coord);
    }
  lastbuttons = mouse_data.buttons;
#endif 
}

/**
 * touch_hide() - hide the mouse cursor
 */
void touch_hide(void)
{
#ifndef __APPLE2__
  mouse_move(screen_w,screen_h);
#endif
}

/**
 * touch_done() - Stop the mouse driver
 */
void touch_done(void)
{
#ifndef __APPLE2__
  if (mouse_present==true)
    mouse_uninstall();
#endif
}
