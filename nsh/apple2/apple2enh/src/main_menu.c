/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Main Menu
 */

#include <stdbool.h>
#include <conio.h>
#include "page.h"
#include "menu.h"
#include "state.h"

#define MENU_X 23
#define MENU_Y 7

extern MainState mainState;

const char *main_menu_header="#FUJINET Network Utilities                                             Main Menu";
const char *main_menu_subheader="Version 1.0                                                            ESC: Quit";

static const char *menu_directory = " 1. Directory of Network Resource ";
static const char *menu_get       = " 2. Receive from Network Resource ";
static const char *menu_put       = " 3. Send to Network Resource "; 
static const char *menu_nc        = " 4. Network Terminal ";
static const char *menu_exit      = " 5. Exit to ProDOS ";

static unsigned char item = 0;
static bool main_menu_exit = false;

void main_menu_select_item(void)
{
  switch(item)
    {
    case 0:
      mainState=DIRECTORY;
      break;
    case 1:
      mainState=GET_URL;
      break;
    case 2:
      mainState=PUT_URL;
      break;
    case 3:
      mainState=NC;
      break;
    case 4:
      mainState=DONE;
      break;
    }
}

void main_menu(void)
{
  unsigned char c;
  
  page(main_menu_header,main_menu_subheader,helpText);

  cputsxy(0,20,selectText);
  cputsxy(0,21,returnText);

  main_menu_exit=false;
  
  while (!main_menu_exit)
    {
      menu_item(item,0,MENU_X,MENU_Y,menu_directory);
      menu_item(item,1,MENU_X,MENU_Y+2,menu_get);
      menu_item(item,2,MENU_X,MENU_Y+4,menu_put);
      menu_item(item,3,MENU_X,MENU_Y+6,menu_nc);
      menu_item(item,4,MENU_X,MENU_Y+8,menu_exit);

      switch(c = cgetc())
	{
	case LEFT:
	case UP:
	  if (item>0)
	    item--;
	  break;
	case RIGHT:
	case DOWN:
	  if (item<4)
	    item++;
	  break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	  item = c-'1';
	  break;
	case ESC:
	  item=4; // Exit to PRODOS
	  break;
	case RETURN:
	  main_menu_select_item();
	  return;
	}
    }
}
