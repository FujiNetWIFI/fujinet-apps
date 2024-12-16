/**
 * @brief   FujiNet News Reader - Commodore 16 Version
 * @author  Thomas Cherryhomes
 * @license gpl v. 3
 * @brief   article
 */

#include <c16.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "article.h"
#include "defines.h"
#include "io.h"

extern State state;
extern long selected_article_id;
extern char buf[BUF_SIZE];
extern const char *urlBase;
extern char nl_delim[2];
extern char page_delim[2];

int article_page;
int i;

void article_footer(void)
{
  // Footer
  textcolor(BCOLOR_CYAN|CATTR_LUMA5);
  revers(1);
  gotoxy(0,24);
  cprintf("CRSR PAGE  I INFO  ESC TITLES   T TOPICS");
  revers(0);
}

State article(void)
{
  char tmp[20];
  char *p;
  unsigned char page_max;
  
  // Small sanity check for testing
  if (!article_page)
    article_page=1;

  // Set up screen
  clrscr();
  bgcolor(BCOLOR_CYAN|CATTR_LUMA1);
  bordercolor(BCOLOR_CYAN|CATTR_LUMA5);
  article_footer();

  // Set up URL
  sprintf(tmp,"&p=%u&a=%lu",article_page,selected_article_id);
  strcat(buf,tmp);

  // Do network
  io_network(tmp);
  
  gotoxy(0,0);

  /* // Deal with header */
  p = strtok(buf,nl_delim);
  p = strtok(NULL, nl_delim);
  p = strtok(NULL, nl_delim);
  p = strtok(NULL, nl_delim);

  /* // Display page */
  gotoxy(0,0);
  revers(1);
  textcolor(BCOLOR_CYAN|CATTR_LUMA5);
  cprintf("%40s",p);
  textcolor(COLOR_CYAN|CATTR_LUMA7);
  strcpy(tmp,p); // store for max page calc below
  revers(0);
  
  /* // Go to content */
  p = strtok(NULL, nl_delim);

  for (i=0;i<20;i++)
    {
      cprintf("%s\r\n",p);
      p = strtok(NULL, nl_delim);
    }

  // Get max page #
  p = strtok(tmp,page_delim);
  p = strtok(NULL,nl_delim);
  page_max = atoi(p);

  while (true)
    {
      switch(cgetc())
	{
	case CH_ESC:
	  state=TITLES;
	  return TITLES;
	case CH_UP:
	  (article_page ? article_page-- : article_page);
	  return ARTICLE;
	case CH_DOWN:
	  (article_page < page_max - 1 ? article_page++ : article_page);
	  return ARTICLE;
	case 't':
	  return TOPICS;
	}
    }

  return ARTICLE;
}
