/**
 * @brief FujiNet Lobby for Adam
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <smartkeys.h>
#include <string.h>
#include "appkey.h"
#include "username.h"
#include "state.h"
#include "input.h"

extern State state;
char _username[32];

void username_set(void)
{
  smartkeys_clear();
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("\n  PLEASE ENTER A USER NAME.");
  input_line(0,19,0,_username,sizeof(_username),NULL);

  if (appkey_write(0x0001,0x01,0x00,_username) != 0x80)
    {
      smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
      smartkeys_status("\n  COULD NOT WRITE APPKEY.");
      state=HALT;
    }
}

void username_get(void)
{
  state=CONNECT;
}

void username(void)
{
  memset(_username,0,sizeof(_username));
  appkey_read(0x0001,0x01,0x00,_username);

  if (_username[0]==0x00)
    username_set();
  else
    username_get();
}
