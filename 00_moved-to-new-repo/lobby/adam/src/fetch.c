/**
 * @brief FujiNet Lobby for Adam
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <eos.h>
#include <stdio.h>
#include <stdlib.h>
#include <smartkeys.h>
#include <stdbool.h>
#include <string.h>
#include "fetch.h"
#include "state.h"
#include "network.h"

#define LOBBY_ENDPOINT "N:http://fujinet.online:8080/view?platform=atari"

extern int page;
extern State state;
extern unsigned char response[1024];

unsigned char buf[1024];
FetchPage fetchPage[FETCH_NUM_PER_PAGE];
int page = 0;
int numEntries = 0;

bool fetch_open(void)
{
  char c = 0;
  
  smartkeys_clear();
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("\n  FETCHING ROOM LIST...");
  smartkeys_sound_play(SOUND_CONFIRM);

  if (network_open(LOBBY_ENDPOINT,MODE_READ_WRITE,2) != ACK)
    {
      smartkeys_clear();
      smartkeys_status("\n  COULD NOT OPEN LOBBY. HALTED.");
      smartkeys_sound_play(SOUND_OOPS);
      state=HALT;
      return false;
    }

  return true;
}

bool fetch_parse(void)
{
  network_set_channel_mode(MODE_JSON);
  
  if (network_parse_json() != ACK)
    {
      smartkeys_clear();
      smartkeys_status("  COULD NOT PARSE ROOMS DATA. HALTED.");
      smartkeys_sound_play(SOUND_OOPS);
      state=HALT;
      return false;
    }

  return true;
}

bool fetch_process(void)
{
  char q[16];

  numEntries=0;
  
  for (int i=0;i<FETCH_NUM_PER_PAGE;i++)
    {
      memset(response,0,sizeof(response));
      sprintf(q,"Q/%u/g",i);
      eos_write_character_device(NET_DEV,q,strlen(q));
      if (eos_read_character_device(NET_DEV,response,1024) != ACK)
	break;
      else
	strcpy(fetchPage[i].g,response);

      memset(response,0,sizeof(response));
      sprintf(q,"Q/%u/t",i);
      eos_write_character_device(NET_DEV,q,strlen(q));
      eos_read_character_device(NET_DEV,response,1024);
      fetchPage[i].t = atoi(response);

      memset(response,0,sizeof(response));
      sprintf(q,"Q/%u/u",i);
      eos_write_character_device(NET_DEV,q,strlen(q));
      eos_read_character_device(NET_DEV,response,1024);
      strcpy(fetchPage[i].u,response);

      memset(response,0,sizeof(response));
      sprintf(q,"Q/%u/c",i);
      eos_write_character_device(NET_DEV,q,strlen(q));
      eos_read_character_device(NET_DEV,response,1024);
      strcpy(fetchPage[i].c,response);

      memset(response,0,sizeof(response));
      sprintf(q,"Q/%u/s",i);
      eos_write_character_device(NET_DEV,q,strlen(q));
      eos_read_character_device(NET_DEV,response,1024);
      strcpy(fetchPage[i].s,response);

      memset(response,0,sizeof(response));
      sprintf(q,"Q/%u/r",i);
      eos_write_character_device(NET_DEV,q,strlen(q));
      eos_read_character_device(NET_DEV,response,1024);
      strcpy(fetchPage[i].r,response);

      memset(response,0,sizeof(response));
      sprintf(q,"Q/%u/o",i);
      eos_write_character_device(NET_DEV,q,strlen(q));
      eos_read_character_device(NET_DEV,response,1024);
      fetchPage[i].o = atoi(response);

      memset(response,0,sizeof(response));
      sprintf(q,"Q/%u/m",i);
      eos_write_character_device(NET_DEV,q,strlen(q));
      eos_read_character_device(NET_DEV,response,1024);
      fetchPage[i].m = atoi(response);

      memset(response,0,sizeof(response)); 
      sprintf(q,"Q/%u/p",i);
      eos_write_character_device(NET_DEV,q,strlen(q));
      eos_read_character_device(NET_DEV,response,1024);
      fetchPage[i].p = atoi(response);

      memset(response,0,sizeof(response));
      sprintf(q,"Q/%u/a",i);
      eos_write_character_device(NET_DEV,q,strlen(q));
      eos_read_character_device(NET_DEV,response,1024);
      fetchPage[i].a = atoi(response);

      numEntries++;
    }
  
  return true;
}

void fetch(void)
{
  state=DISPLAY;

  if (!fetch_open())
    return;

  if (!fetch_parse())
    return;

  if (!fetch_process())
    return;
}
