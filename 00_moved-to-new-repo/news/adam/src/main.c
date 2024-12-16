/**
 * Wikipedia for Adam
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3 
 */

#include <video/tms99x8.h>
#include <eos.h>
#include <smartkeys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "input.h"
#include "font.h"
#include "bar.h"

#define NET 0x09
#define READ 4
#define WRITE 8
#define LF 2
#define ACK 0x80
#define CONNECTED 2

struct _oc
{
  char cmd;
  char mode;
  char trans;
  char url[256];
} OC;

DCB *dcb;

char response[4096];
char selected_category;
char smartkeys_status_msg[128];

enum _state
  {
    CATEGORIES,
    ARTICLE_LIST,
    ARTICLE
  }
state;

const char *categoryNameToNum[] =
  {
    "top",
    "world",
    "business",
    "science",
    "technology",
    "health",
    "entertainment",
    "politics",
    "sports"
  };

const char *urlBase = "N:HTTPS://FUJINET.ONLINE/8bitnews/news.php";

const char delim[2] = "|";

struct _article_list
{
  unsigned long id;
  char date[20];
  char title[128];
} articleList[8];

unsigned char old_i;

unsigned long articleId;
int pageNum, catPageNum;

bool connected(void)
{
  while (eos_request_device_status(NET,dcb) < ACK);
  return eos_get_device_status(NET) & CONNECTED;
}

void display_contents(void)
{
  char r=eos_write_character_device(NET,(unsigned char *)OC,sizeof(OC));

  clrscr();
  
  if (r != ACK)
    {
      smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
      smartkeys_status("  COULD NOT OPEN URL.");
      exit(1);
    }
  
  while (!connected())
    {
      cprintf(".");
      csleep(2);
    }

  clrscr();

  while (connected())
    {
      memset(response,0,sizeof(response));
      
      if (eos_read_character_device(NET,response,1024) != 0x80)
	break;

      cprintf("%s",response);
    }

  // Close the connection
  eos_write_character_device(NET,"C",1);
}

void find_fujinet(void)
{
  dcb = eos_find_dcb(NET);

  if (dcb == NULL)
    {
      smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
      smartkeys_status("  FUJINET NOT FOUND.");
      exit(1);
    }
}

void help(void)
{
  vdp_vfill(MODE2_ATTR+0x0500,0xF4,0x0100);
  vdp_vfill(MODE2_ATTR+0x0600,0x1F,0x0A00);
  vdp_vfill(0x0500,0x00,0x0B00);

  vdp_color(15,4,7);

  gotoxy(0,6); cprintf("%32s","HELP");

  while(1);
}

void categories(void)
{
  smartkeys_set_mode();
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("  WELCOME TO FUJINEWS.");

  pageNum=1;
  
  vdp_vfill(MODE2_ATTR+0x0500,0xF4,0x0100);
  vdp_vfill(MODE2_ATTR+0x0600,0xF5,0x0100);
  vdp_vfill(MODE2_ATTR+0x0700,0x1F,0x0900);
  vdp_vfill(MODE2_ATTR+0x1000,0xF4,0x0100);
  
  cprintf("\x20\x20\x20\x20\x20\x9A\x9B\x9C\x20\x20\x20\x20\x20\n");
  cprintf("\x80\x81\x82\x83\x84\x94\x95\x96\x8A\x8B\x8C\x8D\x8e NEWS READER CLIENT");
  cprintf("\x85\x86\x87\x88\x89\x97\x98\x99\x8F\x90\x91\x92\x93  FOR COLECO  ADAM ");
  cprintf("\x20\x20\x20\x20\x20\x20\x9D\x9E\x9F\x20\x20\x20\x20\n\n");  

  vdp_color(15,4,7);
  
  cprintf("%32s","CATEGORIES");

  vdp_color(1,5,7);
  
  cprintf("%32s","FUJINET.ONLINE/8bitnews/");

  vdp_color(1,15,7);

  cprintf("\xa0\xa1" " TOP STORIES\n");
  cprintf("\xa0\xa1" " WORLD NEWS\n");
  cprintf("\xa0\xa1" " BUSINESS\n");
  cprintf("\xa0\xa1" " SCIENCE\n");
  cprintf("\xa0\xa1" " TECHNOLOGY\n");
  cprintf("\xa0\xa1" " HEALTH\n");
  cprintf("\xa0\xa1" " ENTERTAINMENT\n");
  cprintf("\xa0\xa1" " POLITICS\n");
  cprintf("\xa0\xa1" " SPORTS\n");

  vdp_vfill_v(MODE2_ATTR+0x0700,0xF4,72);
  vdp_vfill_v(MODE2_ATTR+0x0700+8,0xF4,72);

  bar_set(6,2,9,0);

  while (true)
    {
      switch(input())
	{
	case KEY_UP_ARROW:
	  bar_up();
	  break;
	case KEY_DOWN_ARROW:
	  bar_down();
	  break;
	case KEY_RETURN:
	  selected_category=bar_get();
	  state=ARTICLE_LIST;
	  return;
	}
    }
}

void article_list_bar(unsigned char _i)
{
  unsigned short y0=old_i*0x300 + MODE2_ATTR;
  unsigned short y1=_i*0x300 + MODE2_ATTR;

  // Grey out original entry.
  vdp_vfill(y0,0xF1,0x100);
  vdp_vfill(y0+0x100,0x1E,0x200);

  // color new entry
  vdp_vfill(y1,0xF4,0x100);
  vdp_vfill(y1+0x100,0x1F,0x200);  

  old_i=_i;
}

void article_list(void)
{
  char r;      // eos i/o result
  char *page;  // Page strtok ptr
  char *p;     // strtok ptr
  char n=0;    // # of entries
  char i;
  
  smartkeys_set_mode();
  vdp_color(1,14,14);
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("  GETTING ARTICLES, PLEASE WAIT...");

  // Set up URL
  sprintf(OC.url,"%s?t=lf&ps=32x21&l=7&p=%u&c=%s",urlBase,pageNum,categoryNameToNum[selected_category]);

  // Send to FujiNet
  while (eos_write_character_device(NET,(unsigned char *)OC,sizeof(OC)) < 0x80);
  
  // Wait for data to come in
  while (!connected())
    {
      cprintf(".");
      csleep(2);
    }

  // Read article index data
  eos_read_character_device(NET,response,1024);

  // Close connection
  eos_write_character_device(NET,"C",1);

  // Get page marker
  page = strtok(response,"\n");

  // Position to first entry.
  p = strtok(NULL, delim);

  // Tokenize and gather article entries.
  while (p != NULL)
    {
      articleList[n].id = atol(p);

      p = strtok(NULL, delim);
      strncpy(articleList[n].date,p,20);

      p = strtok(NULL, "\n");
      strncpy(articleList[n].title,p,128);
      
      p = strtok(NULL,delim);
      n++;
    }

  // Set up the display
  clrscr();
  
  vdp_vfill(MODE2_ATTR,0x1E,6144); // Fill screen grey initially.

  smartkeys_display(NULL,NULL,NULL,"  PREV\n  PAGE","  NEXT\n  PAGE","CATEGORIES");
  sprintf(smartkeys_status_msg,"  %s %s",categoryNameToNum[selected_category],page);
  smartkeys_status(smartkeys_status_msg);

  for (i=0;i<n;i++)
    {
      vdp_vfill(MODE2_ATTR * 3, 0xF1, 0x100);
      vdp_vfill(MODE2_ATTR * 3 + 0x100, 0x1E, 0x200);
      vdp_color(14,1,7);
      gotoxy(0,i*3); cprintf("%32s",articleList[i].date);
      vdp_color(1,14,7);
      smartkeys_puts(0,i*24+8,articleList[i].title);
    }

  i=0;
  
  article_list_bar(i);
  
  while (1)
    {
      switch(input())
	{
	case KEY_GET:
	case KEY_RETURN:
	  articleId=articleList[i].id;
    catPageNum=pageNum;
	  pageNum=1;
	  state=ARTICLE;
	  return;
	case KEY_ESCAPE:
	case KEY_SMART_VI:
	  state=CATEGORIES;
	  return;
	case KEY_SMART_V:
	  pageNum++;
	  return;
	case KEY_SMART_IV:
	  if (pageNum>1)
	    pageNum--;
	  return;
	case KEY_DOWN_ARROW:
	  if (i<n-1)
	    {
	      i++;
	      article_list_bar(i);
	    }
	  else
	    {
	      pageNum++;
	      return;
	    }
	  break;
	case KEY_UP_ARROW:
	  if (i>0)
	    {
	      i--;
	      article_list_bar(i);
	    }
	  else
	    {
	      if (pageNum>1)
		pageNum--;
	      return;
	    }
	  break;
	}
    }
  
  vdp_vfill(MODE2_ATTR,0xF4,0x100);
  vdp_vfill(MODE2_ATTR+0x100,0x1F,0x200);
}

void article(void)
{
  char r;
  int rp=0;
  char *title, *date, *source, *page, *pageData;
  
  memset(response,0,sizeof(response));
  
  // Set up URL
  sprintf(OC.url,"%s?t=lf&ps=31x18&p=%u&a=%lu",urlBase,pageNum,articleId);

  // Send to FujiNet
  while (eos_write_character_device(NET,(unsigned char *)OC,sizeof(OC)) < 0x80);

  // Wait for data to come in
  while (!connected())
    {
      csleep(2);
    }

  while (connected())
    {
      if (eos_read_character_device(NET,&response[rp],1024) != 0x80)
	break;
      else
	rp += dcb->len;
    }
  
  // Close connection
  eos_write_character_device(NET,"C",1);  

  // Now parse the incoming data
  title    = strtok(response,"\n");
  date     = strtok(NULL,"\n");
  source   = strtok(NULL,"\n");
  page     = strtok(NULL,"\n");
  pageData = strtok(NULL,"\0");

  // Now display the page.
  if (pageNum==1)
    {
      vdp_color(1,14,7);
      clrscr();
      vdp_vfill(MODE2_ATTR,0xF4,0x200);
      vdp_color(15,4,7);
      smartkeys_puts(0,0,title);
    }
  else
    {
      vdp_vfill(0x200,0x00,0x1300);
    }
  
  smartkeys_display(NULL,"  INFO","ARTICLES","  PREV\n  PAGE","  NEXT\n  PAGE","CATEGORIES");
  sprintf(smartkeys_status_msg,"  %s",page);
  smartkeys_status(smartkeys_status_msg);

  vdp_color(1,14,7);
  gotoxy(0,2); cprintf("%s",pageData);

  while(1)
    {
      switch(input())
	{
	case KEY_SMART_II:
	  sprintf(smartkeys_status_msg,"  %8s %s\n  %8s %s","DATE:",date,"SOURCE:",source);
	  smartkeys_display(NULL,NULL,NULL,NULL,NULL," CONTINUE");
	  smartkeys_status(smartkeys_status_msg);
	  while (input() != KEY_SMART_VI); 
	  return;
	case KEY_SMART_III:
    pageNum=catPageNum;
	  state=ARTICLE_LIST;
	  return;
	case KEY_SMART_IV:
	  if (pageNum>1)
	    pageNum--;
	  return;
	case KEY_SMART_V:
	  pageNum++;
	  return;
	case KEY_SMART_VI:
	  state=CATEGORIES;
	  return;
	}
    }
}

void main(void)
{
  font_init();
  smartkeys_set_mode();
  smartkeys_sound_init();
  find_fujinet();
  eos_start_read_keyboard();

  // Go ahead and set up open command
  OC.cmd   = 'O';  // OPEN
  OC.mode  = 0x04; // READ ONLY
  OC.trans = 0x00; // NO TRANSLATION

  while(1)
    {
      switch(state)
	{
	case CATEGORIES:
	  categories();
	  break;
	case ARTICLE_LIST:
	  article_list();
	  break;
	case ARTICLE:
	  article();
	  break;
	}
    }
}
  
