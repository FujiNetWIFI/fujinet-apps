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
#include "pwd.h"

static char *directoryShowSubheader = "URL:                                                          ESC: Select Device";
static char *directorySelectFileText = "Press \xCA \xCB \xC8 \xD5 to select a file.";
static char *directoryPage = "   [...]\r\n";

#define ENTRIES_PER_PAGE              13
#define NERR_EOF                     136

#define MODE_DIRECTORY              0x06
#define MODE_DIRECTORY_80COL_APPLE2 0x81

#define HEADER_Y                    3
#define FILES_Y                     5

#define SPACE                       0x20
#define DEL                         0x7F
#define EOL                         0x9B
#define FOLDER_1                    0xD8
#define FOLDER_2                    0xD9

/**
 * @brief open directory
 * @verbose the parameters are pulled from globals, e.g. url.
 * @param u The unit number (0-3)
 * @return Smartport error, or SP_ERR_NOERROR
 */
unsigned char directory_show_open(unsigned char u)
{
  memset(sp_payload,0,sizeof(sp_payload));
  sp_payload[0] = 4; // 260 byte length
  sp_payload[1] = 1; // ...
  sp_payload[2] = MODE_DIRECTORY;
  sp_payload[3] = MODE_DIRECTORY_80COL_APPLE2;
  sp_payload[4] = 'N';
  sp_payload[5] = ':';
  strcpy((char *)&sp_payload[6],url);
  return sp_control(net[u],'O');
}

/**
 * @brief read directory
 * @verbose This deals entirely with the smartport buffer
 * @param u the unit number (0-3)
 * @param bw return pointer to bytes waiting
 * @param eof return pointer to end of directory bool
 */
unsigned char directory_show_read(unsigned char u, unsigned short *bw, bool *eof)
{
  unsigned char err;
  
  // Get number of bytes waiting and error status
  memset(sp_payload,0,sizeof(sp_payload));
  if (err = sp_status(net[u],'S') != SP_ERR_NOERROR)
    return err;

  (*bw) = sp_payload[0];
  (*bw)|= sp_payload[1] << 8;

  (*eof) = (sp_payload[3] == NERR_EOF);

  if (!(*bw))
    return err;

  return sp_read(net[u],*bw);
}

/**
 * @brief close directory connection
 * @param u The unit number (0-3)
 */
void directory_show_close(unsigned char u)
{
  sp_control(net[u],'C');
}

/**
 * @brief Show the directory page specified
 * @param _page The page # to display
 * @param num_entries Return pointer to # of entries
 * @param next_page Return pointer indicating another page
 * @return false on error. true on success
 */
bool directory_show(unsigned short pg, unsigned char *num_entries, bool *next_page)
{
  unsigned char n=0;
  bool np=true;
  unsigned short se = ENTRIES_PER_PAGE * pg; /* # of entries to skip (if page > 0) */
  
  // Put current prefix + URL in the line buffer, then line buffer into subheader.
  memset(line,0,sizeof(line));
  strncpy(line,pwd(0),80);
  strncat(line,url,80);
  memcpy(&directoryShowSubheader[5],line,strlen(line));
  
  page(directoryHeader,directoryShowSubheader,helpText);
  cputsxy(0,20,directorySelectFileText);
  cputsxy(0,21,returnText);

  if (directory_show_open(selected_unit) != SP_ERR_NOERROR)
    {
      gotoxy(0,6); cprintf("Could not open SmartPort error: %u\nPress any key to return to Main Menu.",directory_show_open(selected_unit));
      cgetc();
      mainState=MAIN_MENU;
      return false;
    }

  cputsxy(2,HEADER_Y,"FILENAME");
  cputsxy(74,HEADER_Y,"BLOCKS");
  
  gotoxy(0,FILES_Y);

  /* Put page up [...], if needed. */
  n++;
  
  if (pg>0)
    {
      cputs(directoryPage);
    }
  else
    gotoxy(0,FILES_Y+1);

  /* Display the entries for selected pg */
  while (n < ENTRIES_PER_PAGE)
    {
      unsigned short bw;                         /* # of bytes waiting in dir buffer*/
      bool eof=false;                            /* dir at EOF? */
      unsigned short i;                          /* dir buffer index */
      
      if (directory_show_read(selected_unit,&bw,&eof) != SP_ERR_NOERROR)
	{
	  gotoxy(0,6); cprintf("Could not read from SmartPort error: %u\nPress any key to return to Main Menu.",directory_show_open(selected_unit));
	  cgetc();
	  mainState=MAIN_MENU;
	  return false;
	}

      // Directory buffer is in sp_payload[] This comes preformatted, but because we need to
      // both skip entries, and deal with broken conio, we have to process it a bit...
      
      for (i=0;i<bw;i++)
	{
	  if ((sp_payload[i] == EOL) && se)
	    {
	      se--;
	      continue;
	    }
	  else if (sp_payload[i] == EOL && n < ENTRIES_PER_PAGE)
	    {
	      n++;
	      cputs("\r\n");
	    }
	  else if ((sp_payload[i]==FOLDER_1) && !se)             // Folder icon
	    cputs("\xD8");                                       // The only reason this is here...
	  else if ((sp_payload[i]==FOLDER_2) && !se)             // ...is because cputc can't display
	    cputs("\xD9");                                       // MouseText, but cputs CAN!
	  else if ((sp_payload[i] < SPACE || sp_payload[i] > DEL) && !se) // only display alphanumeric characters.
	    continue;                                   
	  else if (!se)
	    cputc(sp_payload[i]);                                // Anything else, display.

	  if (n == ENTRIES_PER_PAGE)
	    goto directory_show_page_done;
	}

      if (!bw && eof)
	break;
    }

 directory_show_page_done:
  if (np)
    cputs(directoryPage);
  
  directory_show_close(selected_unit);

  (*num_entries)=n;
  (*next_page)=np;
  
  return true;
}
