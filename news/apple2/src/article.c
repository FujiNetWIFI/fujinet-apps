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

State article(void)
{
  switch (subState)
    {
    case ARTICLE_SHOW:
      break;
    case ARTICLE_PAGE_UP:
      break;
    case ARTICLE_PAGE_DOWN:
      break;
    case ARTICLE_INFO:
      break;
    case ARTICLE_DONE:
      return DONE;
    }

  return ARTICLE;
}
