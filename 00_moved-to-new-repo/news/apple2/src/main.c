/**
 * #FUJINET News
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#include <conio.h>
#include <apple2.h>
#include <stdbool.h>
#include "sp.h"
#include "typedefs.h"
#include "topics.h"
#include "articles.h"
#include "article.h"

int net;

State state;

char url[256];
unsigned char buf[4096];

const char *urlBase = "N:HTTP://FUJINET.ONLINE/8bitnews/news.php";

void main(void)
{
  clrscr();
  sp_init();
  
  net = sp_find_network();

  if (!net)
    {
      cprintf("COULD NOT FIND NETWORK\r\n\r\n");
      cprintf("PRESS ANY KEY TO RETURN\r\n");
      cprintf("TO PRODOS.\r\n");
      
      cgetc();

      return;
    }


  while(1)
    {
      switch (state)
	{
	case TOPICS:
	  state = topics();
	  break;
	case ARTICLES:
	  state = articles();
	  break;
	case ARTICLE:
	  state = article();
	  break;
	case DONE:
	  return;
	}
    }
}
