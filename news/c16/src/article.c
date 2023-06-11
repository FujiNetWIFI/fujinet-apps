/**
 * @brief   FujiNet News Reader - VIC-20 Cart Version
 * @author  Thomas Cherryhomes
 * @license gpl v. 3
 * @brief   article
 */

#include <vic20.h>
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
extern char buf[512];
extern const char *urlBase;
extern char nl_delim[2];
extern char page_delim[2];

int article_page;
int i;

void article_footer(void)
{
  // Footer
  textcolor(COLOR_RED);
  revers(1);
  gotoxy(0,21);
  cprintf("UP/DN PAGE      I INFO_ TITLES      T TOPICS");
  gotoxy(0,22);
  cbm_k_bsout(0x5F); // this is silly.
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
  VIC.bg_border_color = 250; 
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
  textcolor(COLOR_RED);
  cprintf("%22s",p);
  textcolor(COLOR_BLACK);
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
	case CH_LEFT:
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
