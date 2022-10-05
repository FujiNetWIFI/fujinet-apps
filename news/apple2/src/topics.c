/**
 * #FUJINET News
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#include <apple2.h>
#include <conio.h>
#include "sp.h"
#include "topics.h"
#include "typedefs.h"
#include "bar.h"

static TopicsState subState=TOPICS_SHOW;

static State newState=TOPICS;

const char *topicNames[] = {
  "TOP STORIES",
  "WORLD NEWS",
  "BUSINESS",
  "SCIENCE",
  "TECHNOLOGY",
  "HEALTH",
  "ENTERTAINMENT",
  "POLITICS",
  "SPORTS"
};

unsigned char selectedTopic=0;

TopicsState topics_show(void)
{
  char y=7;

  clrscr();

  cprintf("#FUJINET NEWS FOR APPLE II  -  VERS. 1.0SERVER: HTTPS://FUJINET.ONLINE/8BITNEWS/");
  
  /* Put rules around topics */

  gotoxy(0,5);
  chline(40);

  gotoxy(0,15);
  chline(40);

  /* Print topics */
  
  for (y=0;y<7;y++)
    {
      gotoxy(0,y+7);
      cprintf("[%u] %s",y+1,topicNames[y]);
    }

  /* Print help */

  gotoxy(0,22); cprintf("[ARROW KEYS] MOVE BAR [1-7] JUMP TOPIC");
  gotoxy(0,23); cprintf("[RETURN] PICK TOPIC   [ESC] EXIT");
  
  /* Set up bar */
  bar_set(7,1,7,0);
  bar_update();

  return TOPICS_INPUT;
}

TopicsState topics_input(void)
{
  TopicsState newSubState=TOPICS_INPUT;
  char k;
  
  switch(k=cgetc())
    {
    case KEY_1:
    case KEY_2:
    case KEY_3:
    case KEY_4:
    case KEY_5:
    case KEY_6:
    case KEY_7:
      bar_jump(k-0x31);
      break;
    case KEY_UP_ARROW:
      bar_up();
      break;
    case KEY_DOWN_ARROW:
      bar_down();
      break;
    case KEY_ESCAPE:
      newState=DONE;
      break;
    case KEY_RETURN:
      selectedTopic=bar_get();
      newState=ARTICLES;
    }

  return newSubState;
}

State topics(void)
{
  switch (subState)
    {
    case TOPICS_SHOW:
      subState=topics_show();
      break;
    case TOPICS_INPUT:
      subState=topics_input();
      break;
    }

  return newState;
}
