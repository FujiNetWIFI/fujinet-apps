/**
 * @brief FujiNet Lobby for Adam
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <stdbool.h>
#include "state.h"
#include "init.h"
#include "username.h"
#include "connect.h"
#include "fetch.h"
#include "display.h"
#include "select.h"
#include "shout.h"
#include "chat.h"

State state = INIT;

void main(void)
{
  while (true)
    {
      switch(state)
	{
	case INIT:
	  init();
	  break;
	case USERNAME:
	  username();
	  break;
	case CONNECT:
	  connect();
	  break;
	case FETCH:
	  fetch();
	  break;
	case DISPLAY:
	  display();
	  break;
	case SELECT:
	  select();
	  break;
	case SHOUT:
	  shout();
	  break;
	case CHAT:
	  chat();
	  break;
	}
    }
}
