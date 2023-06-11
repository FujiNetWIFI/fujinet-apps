/**
 * @brief   FujiNet News Reader - Commodore 16 Version
 * @author  Thomas Cherryhomes
 * @license gpl v. 3
 * @brief   main
 */

#include <c16.h>
#include <cbm.h>
#include <conio.h>
#include <stdbool.h>
#include "defines.h"
#include "topics.h"
#include "titles.h"
#include "article.h"
#include "io.h"

State state=TOPICS;

char buf[BUF_SIZE];

const char nl_delim[2] = {0x0A, 0x00};
const char page_delim[2] = {'/', 0x00};

void main(void)
{
  while(true)
    {
      io_init();
      switch(state)
	{
	case TOPICS:
	  state=topics();
	  break;
	case TITLES:
	  state=titles();
	  break;
	case ARTICLE:
	  state=article();
	  break;
	}
    }
}
