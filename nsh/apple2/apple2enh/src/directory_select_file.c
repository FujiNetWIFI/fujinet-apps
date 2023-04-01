/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Directory Show
 */

#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include "sp.h"
#include "select_device.h"
#include "directory.h"
#include "state.h"
#include "menu.h"
#include "page.h"

#define DIRECTORY_SELECT_FILE_OFFSET 5
#define CH_M                         0x4D
#define CH_M_INVERSE                 0x8D

static unsigned char i,oldi;

/**
 * @brief cputc that works around a weird bug in conio.
 * @param fix apply inverse->normal fix.
 */
void fixed_cputc(bool fix)
{
  if (fix)
    cputc(cpeekc() == CH_M_INVERSE ? CH_M : cpeekc()^0x80);
  else
    cputc(cpeekc());
}

/**
 * @brief Reset directory view
 */
void directory_select_file_reset(void)
{
  i=1;
  oldi=0xFF;
}

/**
 * @brief Navigate/select file
 * @param selected_page The currently visible page (e.g. for previous page [...])
 * @param num_entries The number of entries visible
 * @param is next_page visible?
 * @return the selected file in global variable 'url'
 */
void directory_select_file(unsigned char *selected_page, unsigned char num_entries, bool next_page)
{
  unsigned char c=0,o=DIRECTORY_SELECT_FILE_OFFSET;

  // Delete old cursor
  if (oldi!=0xFF)
    {
      for (c=2;c<80;c++)
	{
	  gotoxy(c,oldi+o);
	  fixed_cputc(true);
	}
    }

  revers(1);

  for (c=2;c<80;c++)
    {
      gotoxy(c,i+o);
      fixed_cputc(false);
    }

  oldi = i;
  
  revers(0);

  switch (c=cgetc())
    {
    case 0x15:
    case 0x0A:
      if (i<num_entries)
	i++;
      break;
    case 0x08:
    case 0x0B:
      if (i>0)
	i--;
      break;
    case 0x1b:
      directoryState=SELECT_DEVICE;
      break;
    case 0x0d:
      break;
    }

  if (i==num_entries && next_page)
    {
      (*selected_page)++;
      directoryState=SHOW_DIRECTORY;
    }
  else if (i==0 && (*selected_page)>0)
    {
      (*selected_page)--;
      directoryState=SHOW_DIRECTORY;
    }
}
