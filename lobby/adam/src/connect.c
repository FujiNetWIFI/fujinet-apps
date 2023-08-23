/**
 * @brief FujiNet Lobby for Adam
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <eos.h>
#include <string.h>
#include <stdio.h>
#include "connect.h"
#include "state.h"

#define CHAT_DEV 0x0A

extern State state;

const char connect_cmd[] = "O\x0c\x02N:TCP://fujinet.online:7373/";

void connect(void)
{
  char t[256];
  eos_write_character_device(CHAT_DEV,connect_cmd,strlen(connect_cmd));
  sprintf(t,"/login @%s\n\n",username);
  eos_write_character_device(CHAT_DEV,t,strlen(t));
  memset(t,0,sizeof(t));
  sprintf(t,"/join #lobby\n");
  eos_write_character_device(CHAT_DEV,t,strlen(t));
  state=FETCH;
}
