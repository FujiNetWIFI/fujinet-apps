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
#include "articles.h"
#include "typedefs.h"

static ArticlesState subState=ARTICLES_SHOW;

ArticlesState articles_show(void)
{
  return ARTICLES_SHOW;
}

ArticlesState articles_up(void)
{
  return ARTICLES_SHOW;
}

ArticlesState articles_down(void)
{
  return ARTICLES_SHOW;
}

ArticlesState articles_page_up(void)
{
  return ARTICLES_SHOW;
}

ArticlesState articles_page_down(void)
{
  return ARTICLES_SHOW;
}

State articles(void)
{
  switch (subState)
    {
    case ARTICLES_SHOW:
      subState=articles_show();
      break;
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
