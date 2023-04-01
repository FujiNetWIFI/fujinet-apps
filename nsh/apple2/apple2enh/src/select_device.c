/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Select Network Device
 */

#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdbool.h>
#include "page.h"
#include "menu.h"
#include "pwd.h"
#include "state.h"
#include "cd.h"
#include "input.h"

extern MainState mainState;
extern unsigned char net[4]; /* TEMPORARY */
extern char line[80];

static bool select_device_menu_exit=false;

static const char *prefixSetText="This prefix is empty. Please enter the desired prefix for this slot";

unsigned char selected_unit=0;

#define SELECT_DEVICE_X 0
#define SELECT_DEVICE_Y 7

void select_device_select_item(void)
{
  char s[128];

  memset(s,0,sizeof(s));

  if (pwd(selected_unit)[0]=='<')
    {
      sprintf(line," %d.                                                                            ",selected_unit+1);
      menu_item(0xFF,selected_unit,SELECT_DEVICE_X,SELECT_DEVICE_Y+(selected_unit<<1),line);
      cputsxy(0,20,prefixSetText);
      gotoxy(SELECT_DEVICE_X+4,SELECT_DEVICE_Y+(selected_unit<<1));
      input(s);

      if (s[0]==0x00)
	{
	  mainState=MAIN_MENU;
	  return;
	}

      cd(selected_unit,s);
    }
}

unsigned char select_device(const char *header)
{
  unsigned char u,c;
  
  page(header,selectDeviceSubheader,helpText);

  cputsxy(0,20,selectText);
  cputsxy(0,21,returnText);

  select_device_menu_exit=false;
  
  while (!select_device_menu_exit)
    {
      /* Poll all units for current prefixes, and display them. */
      for (u=0;u<4;u++)
	{
	  sprintf(line," %d. %-75s",u+1,pwd(u));
	  menu_item(selected_unit,u,SELECT_DEVICE_X,SELECT_DEVICE_Y+(u<<1),line);
	}

      switch(c = cgetc())
	{
	case LEFT:
	case UP:
	  if (selected_unit>0)
	    selected_unit--;
	  break;
	case RIGHT:
	case DOWN:
	  if (selected_unit<3)
	    selected_unit++;
	  break;
	case '1':
	case '2':
	case '3':
	case '4':
	  selected_unit=c-'1';
	  break;
	case ESC:
	  select_device_menu_exit=true;
	  mainState=MAIN_MENU;
	  break;
	case RETURN:
	  select_device_menu_exit=true;
	  select_device_select_item();
	  break;
	}
    }

  return net[0]; /* TEMPORARY */
}
