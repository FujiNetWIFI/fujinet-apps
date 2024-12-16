/**
 * @brief   FujiNet News Reader - Commodore 16 Version
 * @author  Thomas Cherryhomes
 * @license gpl v. 3
 * @brief   titles
 */

#include <stdio.h>
#include <stdlib.h>
#include <c16.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include "titles.h"
#include "io.h"

extern State state;
extern char buf[BUF_SIZE];
extern unsigned char selected_topic;
extern int article_page;
extern char nl_delim[2];
extern char page_delim[2];

static int title_page_num;

char selected_article;
unsigned long selected_article_id;
char tmp[128];

const char delim[2] = {0x7C,0x00};

const char *topic_ids[TOPICS_MAX] =
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

  struct _a
  {
    long int id;
    char date[20];
    char title[120];
  } a[5];

State titles(void)
{
  char *p;
  char i;
  char pg[8];

  if (!title_page_num)
    title_page_num=1;
  
  bgcolor(BCOLOR_BLUE|CATTR_LUMA1);
  bordercolor(BCOLOR_BLUE|CATTR_LUMA5);
  textcolor(BCOLOR_BLUE|CATTR_LUMA7);

  clrscr();
  gotoxy(0,11);
  cprintf("      FETCHING  ARTICLES     ");

  // Set up URL.
  sprintf(tmp,"&p=%u&c=%s",title_page_num,topic_ids[selected_topic]);

  io_network(tmp);
  
  selected_article=i=0;

  p = strtok(buf,nl_delim);
  strcpy(pg,p);
    
  // Position to first entry
  p = strtok(NULL, delim);
  
  // Parse entries
  while (p)
    {
      memset(a[i].date,0,20);
      memset(a[i].title,0,120);
      
      a[i].id = atol(p);

      p = strtok(NULL,delim);
      strncpy(a[i].date,p,20);

      p = strtok(NULL,nl_delim);
      strncpy(a[i++].title,p,119);

      p = strtok(NULL,delim);
    }

  clrscr();

  revers(1);
  textcolor(BCOLOR_BLUE|CATTR_LUMA5);
  cputs(TITLES_TOP);
  gotoxy(0,24);
  cputs(TITLES_BOT);
  
  revers(0);

  while(true)
    {
      
      gotoxy(0,TITLES_Y);
      
      if (!title_page_num)
	title_page_num=1;

      textcolor(BCOLOR_GREEN|CATTR_LUMA4);  
      cprintf("\n%40s\n",pg);

      for (i=0;i<TITLES_MAX;i++)
	{
	  revers(selected_article==i);
	  textcolor(selected_article==i ? BCOLOR_BLUE|CATTR_LUMA6 : BCOLOR_BLUE|CATTR_LUMA3);
	  cprintf("%40s",a[i].date);
	  textcolor(BCOLOR_WHITE|CATTR_LUMA6);
	  cprintf("%-120s",a[i].title);
	}
      
      switch(cgetc())
	{
	case CH_DOWN:
	  ( selected_article < TITLES_MAX - 1 ? selected_article++ : selected_article);
	  break;
	case CH_UP:
	  ( selected_article ? selected_article-- : selected_article );
	  break;
	case CH_RT:
	  title_page_num++;
	  return TITLES;
	case CH_LF:
	  if (title_page_num)
	    title_page_num--;
	  return TITLES;
	case CH_RETURN:
	  selected_article_id=a[selected_article].id;
	  article_page=1;
	  state=ARTICLE;
	  return ARTICLE;
	case CH_ESC:
	  state=TOPICS;
	  return TOPICS;
	}
    }
  
  return state;
}
