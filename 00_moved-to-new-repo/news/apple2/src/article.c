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
#include "article.h"
#include "typedefs.h"

static ArticleState subState=ARTICLE_SHOW;

ArticleState article_show(void)
{
  return ARTICLE_SHOW;
}

ArticleState article_page_up(void)
{
  return ARTICLE_SHOW;
}
ArticleState article_page_down(void)
{
  return ARTICLE_SHOW;
}
ArticleState article_info(void)
{
  return ARTICLE_SHOW;
}

State article(void)
{
  switch (subState)
    {
    case ARTICLE_SHOW:
      subState=article_show();
      break;
    case ARTICLE_PAGE_UP:
      subState=article_page_up();
      break;
    case ARTICLE_PAGE_DOWN:
      subState=article_page_down();
      break;
    case ARTICLE_INFO:
      subState=article_info();
      break;
    case ARTICLE_DONE:
      return DONE;
    }

  return ARTICLE;
}
