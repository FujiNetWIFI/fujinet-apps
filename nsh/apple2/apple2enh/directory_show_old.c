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
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sp.h"
#include "state.h"
#include "menu.h"
#include "page.h"
#include "pwd.h"
#include "directory.h"
#include "state.h"

extern DirectoryState directoryState;
static unsigned char current_page=0;
static unsigned char _cursor=0, _oldCursor=0xFF;
static unsigned char line_counter=0;
extern const char *directoryHeader;
extern MainState mainState;

static char urlDisplay[76];

static char *directoryShowSubheader = "URL:                                                          ESC: Select Device";
static char *directorySelectFileText = "Press \xCA \xCB \xC8 \xD5 to select a file.";
static char *directoryPage = "   [...]\r\n";

#define LINES_PER_PAGE                12
#define MODE_DIRECTORY              0x06
#define MODE_DIRECTORY_80COL_APPLE2 0x81
#define NERR_EOF                     136

/**
 * @brief return filename at cursor, and whether it is a folder.
 * @param return pointer for filename string
 * @param return pointer for folder flag.
 */
void directory_read_entry_under_cursor(char *s, bool *f)
{
  unsigned char i,j,c,o=5;

  if (current_page==0)
    o++;

  i=68; // Find end of filename
  while (j > 2)
    {      
      j--;
      
      if (c==0x20)
	break;
    }

  for (i=3;i<j;i++)
    {
      gotoxy(i,_cursor+o);
      c=cpeekc() == 0x8D ? 0x4D : cpeekc()^0x80;

      if (c==0x20)
	break;
      else
	s[i-3]=c;
    }

  // Get folder attribute
  gotoxy(0,_cursor+o);
  c=cpeekc();
  *f = (c == 'X');
}

/**
 * @brief update directory cursor.
 */
void directory_show_cursor(void)
{
  unsigned char j;
  unsigned char o=5;
  
  /* move cursor down one if on first page. */
  if (current_page==0)
    o++;

  if (_oldCursor!=0xFF) // Turn off the old cursor.
    {
      revers(0);
      for (j=2;j<80;j++)
	{
	  gotoxy(j,_oldCursor+o);
	  cputc(cpeekc() == 0x8D ? 0x4D : cpeekc()^0x80); // The fix. OK, WHY?!
	}
    }    

  revers(1);

  for (j=2;j<80;j++) // and put the new one on screen.
    {
      gotoxy(j,_cursor+o);
      cputc(cpeekc());
    }

  _oldCursor = _cursor;

  revers(0);
}

/**
 * @brief Reset directory show view
 */
void directory_show_reset(void)
{
  current_page=_cursor=0;
  _oldCursor=0xFF;
}

/**
 * @brief Show directory page
 */
void directory_show(void)
{
  unsigned char err; // SmartPort error
  unsigned short bw; // # of bytes waiting in directory buffer
  unsigned char nerr; // Network error code
  bool show_next_page=true; // Do we show the [...] for next page?
  unsigned short skipEntries = current_page * LINES_PER_PAGE; // # of dir entries to skip (relative to beginning)
  char c; // Selected keypress
  char l[70]; // Line buffer for selected filename.
  bool f; // Is folder?
  
  /** RETRIEVE DIRECTORY PAGE **********************************************************/
  memset(l,0,sizeof(l));
  memset(urlDisplay,0,sizeof(urlDisplay));
  strncpy(urlDisplay,pwd(0),75);
  strncat(urlDisplay,url,75);
  
  memcpy(&directoryShowSubheader[5],urlDisplay,strlen(urlDisplay));

  page(directoryHeader,directoryShowSubheader,helpText);

  cputsxy(0,20,directorySelectFileText);
  cputsxy(0,21,returnText);
  
  memset(sp_payload,0,sizeof(sp_payload));
  
  memset(sp_payload,0,sizeof(sp_payload));
  line_counter=0;
  
  sp_payload[0] = 4; // 260 byte length
  sp_payload[1] = 1; // ...
  sp_payload[2] = MODE_DIRECTORY;
  sp_payload[3] = MODE_DIRECTORY_80COL_APPLE2;
  sp_payload[4] = 'N';
  sp_payload[5] = ':';
  strcpy((char *)&sp_payload[6],url);
  err = sp_control(net[0],'O');

  if (err != SP_ERR_NOERROR)
    {
      gotoxy(0,6); cprintf("Could not open SmartPort error: %u\nPress any key to return to Main Menu.",err);
      cgetc();
      mainState=MAIN_MENU;
      return;
    }

  gotoxy(0,3);

  cprintf("  FILENAME                                                                BLOCKS");

  gotoxy(0,5);

  if (current_page>0)
    {
      line_counter++;
      cputs(directoryPage);
    }
  else
    cputs("\n");
  
  while (line_counter<LINES_PER_PAGE)
    {
      unsigned short i; // directory buffer index
      
      sp_status(net[0],'S');
      
      bw = sp_payload[0];
      bw |= sp_payload[1] << 8;

      nerr = sp_payload[3];

      if (bw>0)
	{
	  sp_read(net[0],bw);
	  
	  for (i=0;i<bw;i++)
	    {
	      if (sp_payload[i]==0x9B)
		{
		  if (skipEntries)
		    {
		      skipEntries--;
		      continue;
		    }
		  
		  line_counter++;
		  
		  cputc('\r');
		  cputc('\n');
		}
	      else if (sp_payload[i]==0xD8) // Folder icon
		{
		  if (!skipEntries)
		    cputs("\xD8\xD9"); // Due to how brain damaged cputc is.
		}
	      else if (sp_payload[i]==0xD9)
		continue;
	      else if (sp_payload[i]<32 || sp_payload[i]>127)
		continue;
	      else if (line_counter==LINES_PER_PAGE)
		break;
	      else if (!skipEntries)
		cputc(sp_payload[i]);
	    }
	}

      if (nerr == NERR_EOF && !bw)
	{
	  show_next_page=false;
	  break;
	}
    }

  if (show_next_page)
    cputs(directoryPage);

  sp_control(net[0],'C');

  /** NAVIGATE DIRECTORY PAGE *******************************************************/

  while(directoryState==SHOW_DIRECTORY)
    {
      memset(l,0,sizeof(l));
      
      directory_show_cursor();

      switch (c=cgetc())
	{
	case 0x15:
	case 0x0A:
	  if (_cursor<line_counter)
	    _cursor++;
	  break;
	case 0x08:
	case 0x0B:
	  if (_cursor>0)
	    _cursor--;
	  break;
	case 0x1b:
	  directoryState=SELECT_DEVICE;
	  break;
	case 0x0d:
	  directory_read_entry_under_cursor(l,&f);
	  cputsxy(0,19,l);
	  break;
	}
    }
}
