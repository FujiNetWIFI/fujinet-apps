/**
 * #FUJINET News
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#include <apple2.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sp.h"
#include "articles.h"
#include "typedefs.h"

const char delim[2] = "|";
const char lapw[]="     LOADING ARTICLES, PLEASE WAIT.    ";

static const char *topicNames[] =
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

extern const char *urlBase;
extern char url[256];
extern int net;
extern unsigned char selectedTopic;
extern State state;
extern unsigned char buf[4096];

static ArticlesState subState=ARTICLES_INIT;
static unsigned char num_articles=0;
static unsigned short pageNum=1;

struct _article_list
{
  long int id;
  char date[20];
  char title[128];
} articleList[8];

ArticlesState articles_init(void)
{
  num_articles=0;
  pageNum=1;
  memset(buf,0,sizeof(buf));
  return ARTICLES_LOAD;
}

ArticlesState articles_load(void)
{
  ArticlesState newSubState=ARTICLES_DONE;
  unsigned short i=0;
  unsigned short bw;      /* bytes waiting */

  clrscr();
  gotoxy(0,12); cprintf("%s",lapw);

  memset(url,0,sizeof(url));
  snprintf(url,sizeof(url),"%s?t=lf&ps=40x21&l=7&p=%u&c=%s",urlBase,pageNum,topicNames[selectedTopic]);
  
  sp_open(net);

  sp_payload[0]=0x02;
  sp_payload[1]=0x01;
  sp_payload[2]=0x0C;
  sp_payload[3]=0x80;
  
  memcpy(&sp_payload[4],url,sizeof(sp_payload));

  sp_control(net,'O');

  sp_status(net,'S'); // Get status
  
  while((bw = (unsigned short)sp_payload[0]) > 0)
    {
      if (bw>512)
	bw=512;
      
      sp_read(net,bw);
      memcpy(&buf[i],sp_payload,bw);
      i+=bw;
      sp_status(net,'S');
    }
    
  sp_close(net);

  return ARTICLES_SHOW;
}

ArticlesState articles_show(void)
{
  char *page = strtok((char *)buf,"\n");
  char *p;
  char i;
  
  // Parse article page
  
  p = strtok(NULL,delim);

  while (p != NULL)
    {
      articleList[num_articles].id = atol(p);

      p = strtok(NULL,delim);
      strncpy(articleList[num_articles].date,p,20);

      p = strtok(NULL,delim);
      strncpy(articleList[num_articles].title,p,128);

      if (strlen(articleList[num_articles].title)>125)
	{
	  articleList[num_articles].title[125]='.';
	  articleList[num_articles].title[126]='.';
	  articleList[num_articles].title[127]='.';	  
	}
      
      p = strtok(NULL,delim);
      num_articles++;
    }

  // Display article data

  clrscr();
  
  gotoxy(0,0); cprintf("PAGE: %s\r\n\r\n",page);
  
  for (i=0;i<num_articles;i++)
    {
      cprintf("%40s",articleList[i].date);
    }
  
  return ARTICLES_INPUT;
}

ArticlesState articles_input(void)
{
  return ARTICLES_INPUT;
}

ArticlesState articles_up(void)
{
  return ARTICLES_INPUT;
}

ArticlesState articles_down(void)
{
  return ARTICLES_INPUT;
}

ArticlesState articles_page_up(void)
{
  return ARTICLES_INPUT;
}

ArticlesState articles_page_down(void)
{
  return ARTICLES_INPUT;
}

State articles(void)
{
  switch (subState)
    {
    case ARTICLES_INIT:
      subState=articles_init();
      break;
    case ARTICLES_LOAD:
      subState=articles_load();
      break;
    case ARTICLES_SHOW:
      subState=articles_show();
      break;
    case ARTICLES_INPUT:
      subState=articles_input();
    case ARTICLES_UP:
      subState=articles_up();
      break;
    case ARTICLES_DOWN:
      subState=articles_down();
      break;
    case ARTICLES_PAGE_UP:
      subState=articles_page_up();
      break;
    case ARTICLES_PAGE_DOWN:
      subState=articles_page_down();
      break;
    case ARTICLES_DONE:
      return DONE;
    }

  return ARTICLES;
}
