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

State articles(void)
{
  switch (subState)
    {
    case ARTICLES_SHOW:
      break;
    case ARTICLES_UP:
      break;
    case ARTICLES_DOWN:
      break;
    case ARTICLES_PAGE_UP:
      break;
    case ARTICLES_PAGE_DOWN:
      break;
    case ARTICLES_DONE:
      return DONE;
    }

  return ARTICLES;
}
