/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <stdbool.h>
#include <string.h>
#include "select_file.h"
#include "system.h"
#include "typedefs.h"
#include "bar.h"

extern char selected_host_slot;
extern HostSlots hs;

#define ENTRIES_PER_PAGE 15
#define ENTRY_TIMER_DUR 128
#define DIR_MAX_LEN 31

SFState sfState;
char path[224];
char filter[32];
DirectoryPosition pos=0;
bool dir_eof=false;
unsigned long selected_size=0;
unsigned char entry_size[ENTRIES_PER_PAGE];
unsigned short entry_timer=ENTRY_TIMER_DUR;
bool long_entry_displayed=false;

void select_file_init(void)
{
  io_close_directory();
  pos=0;
  memset(entry_size,0,ENTRIES_PER_PAGE);
  memset(path,0,256);
  path[0]='/';
  memset(filter,0,32);
  screen_select_file();
  sfState=SF_DISPLAY;
  dir_eof=false;
}

unsigned char select_file_display(void)
{
  char visibleEntries = 0;
  char i;
  char *e;
  
  io_mount_host_slot(selected_host_slot);

  screen_select_file_display(path,filter,hs[selected_host_slot]);
  
  io_open_directory(selected_host_slot,path,filter);
  
  if (pos > 0)
    io_set_directory_position(pos);

  for (i = 0; i < ENTRIES_PER_PAGE; i++)
  {
    e = io_read_directory(DIR_MAX_LEN, 0);
    if (e[2] == 0x7F)
    {
      dir_eof = true;
      break;
    }
    else
    {
      entry_size[i] = strlen(e);
      visibleEntries++; // could filter on e[0] to deal with message entries like on FUJINET.PL
      screen_select_file_display_entry(i, e);
    }
  }

  // Do one more read to check EOF
  e = io_read_directory(DIR_MAX_LEN, 0);
  if (e[2] == 0x7F)
    dir_eof = true;

  io_close_directory();

  if (pos > 0)
    screen_select_file_prev();

  if (dir_eof != true)
    screen_select_file_next();

  sfState = SF_CHOOSE;
  return visibleEntries;
}

void select_file_display_long_filename(void)
{
  char *e;
  
  if ((entry_size[bar_get()]>30) && (entry_timer == 0))
    {
      if (long_entry_displayed==false)
	{
	  io_open_directory(selected_host_slot,path,filter);
	  io_set_directory_position(pos+bar_get());
	  e = io_read_directory(64,0);
	  screen_select_file_display_long_filename(e);
	  io_close_directory();
	  long_entry_displayed=true;
	}
    }
  else
    {
      long_entry_displayed=false;
      screen_select_file_clear_long_filename();
    }
}

void select_file_choose(char visibleEntries)
{
  char k=0;
  
  screen_select_file_choose(visibleEntries);
  bar_init(3,2,visibleEntries,0); // TODO: Handle previous
  
  while (sfState==SF_CHOOSE)
    {
      sfState=input_select_file_choose();
      select_file_display_long_filename();
    }
}

bool select_file_is_folder(void)
{
  char *e;
  bool result;

  io_open_directory(selected_host_slot,path,filter);

  io_set_directory_position(pos);

  e = io_read_directory(128,0);

  result = (strrchr(e,'/') != NULL);

  io_close_directory();
 
  return result; // Offset 10 = directory flag.
}

void select_file_next_page(void)
{
  bar_done();
  pos += ENTRIES_PER_PAGE;
  sfState=SF_DISPLAY;
  dir_eof=false;
}

void select_file_prev_page(void)
{
  bar_done();
  pos -= ENTRIES_PER_PAGE;
  sfState=SF_DISPLAY;
  dir_eof=false;
}

void select_file_filter(void)
{
  memset(filter,0,32);
  screen_select_file_filter();
  input_line_filter(filter);
  
  dir_eof=false;
  pos=0;

  sfState=SF_DISPLAY;
}

void select_file_advance(void)
{
  char *e;

  bar_done();
  
  io_open_directory(selected_host_slot,path,filter);

  io_set_directory_position(pos);
  
  e = io_read_directory(128,1);
  
  strcat(path,e); // append directory entry to end of current path

  io_close_directory(); // have to use "e" before calling another io command, otherwise e gets wiped out

  pos=0;
  dir_eof=false;
  
  sfState=SF_DISPLAY; // and display the result.
}

void select_file_devance(void)
{
  char *p = strrchr(path,'/'); // find end of directory string (last /)

  bar_done();

  while (*--p != '/'); // scoot backward until we reach next /

  p++;
  
  *p = 0; // truncate string.

  pos=0;
  dir_eof=false;
  
  sfState=SF_DISPLAY; // And display the result.
}

State select_file(void)
{
  char visibleEntries=0;
  
  screen_select_file();

  while(1)
    {
      switch(sfState)
	{
	case SF_INIT:
	  select_file_init();
	  break;
	case SF_DISPLAY:
	  visibleEntries=select_file_display();
	  break;
	case SF_NEXT_PAGE:
	  select_file_next_page();
	  break;
	case SF_PREV_PAGE:
	  select_file_prev_page();
	  break;
	case SF_CHOOSE:
	  select_file_choose(visibleEntries);
	  break;
	case SF_FILTER:
	  select_file_filter();
	  break;
	case SF_ADVANCE_FOLDER:
	  select_file_advance();
	  break;
	case SF_DEVANCE_FOLDER:
	  select_file_devance();
	  break;
	case SF_DONE:
	  break;
	}
    }
  
  return SELECT_TAPE;
}
