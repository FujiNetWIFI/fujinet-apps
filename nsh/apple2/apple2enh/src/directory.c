/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Directory
 */

#include <conio.h>
#include <string.h>
#include <stdio.h>
#include "directory.h"
#include "page.h"
#include "menu.h"
#include "select_device.h"
#include "state.h"
#include "directory_show.h"
#include "directory_select_file.h"

extern MainState mainState;
extern char line[80];
const char *directoryHeader="#FUJINET Network Utilities                                             Directory";
char url[256];
unsigned char num_entries;
bool next_page;
unsigned char selected_page=0;

DirectoryState directoryState=SELECT_DEVICE;

void directory(void)
{
  directoryState=SELECT_DEVICE;

  selected_page=num_entries=next_page=0;
  selected_page=0;
  memset(url,0,sizeof(url));
  
  while (mainState == DIRECTORY)
    {
      switch (directoryState)
	{
	case SELECT_DEVICE:
	  select_device(directoryHeader);
	  directoryState=SHOW_DIRECTORY;
	  break;
	case SHOW_DIRECTORY:
	  if (directory_show(selected_page,&num_entries,&next_page))
	    directoryState=SELECT_FILE;
	  directory_select_file_reset();
	case SELECT_FILE:
	  directory_select_file(&selected_page,num_entries,next_page);
	  break;
	}
    }
}
