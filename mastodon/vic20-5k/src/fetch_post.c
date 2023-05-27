
#include <cbm.h>
#include <string.h>
#include <stdlib.h>
#include <peekpoke.h>
#include "video.h"

#define LFN 2     // The logical file number to use for I/O
#define DEV 12    // The network device #
#define SAN 2     // The secondary address (SA) to use on DEV.
#define CMD 15    // The secondary address for the Command channel

extern char display_name[23], created_at[23], content[400];

const char url[]="https://oldbytes.space/api/v1/timelines/public?limit=1";
const char cmd_parse[]="jsonparse,2";
const char display_name_query[]="jq,2,/0/account/display_name";
const char created_at_query[]="jq,2,/0/created_at";
const char content_query[]="jq,2,/0/content";

/**
 * @brief read data into tmp, cleaning it first.
 */
void read_into_content(void)
{
  memset(content,0,sizeof(content));
  cbm_read(LFN,content,sizeof(content));
}

/**
 * @brief Fetch Mastodon Post
 * @param display_name The name to display
 * @param created_at The created at timestamp to display
 * @param content The content to display
 * @param replies_count # of replies to thread
 * @param reblogs_count # of boosts
 * @param favorites_count # of hearts
 */
void fetch_post(void)
{
  print("\x13\x1F\x12"" FETCHING LATEST POST\x92");

  POKE(0xCC,0);

  // Open command channel
  cbm_open(CMD,DEV,CMD,"");

  // Open channel for Mastodon URL
  cbm_open(LFN,DEV,SAN,url);

  // Ask FujiNet to parse incoming JSON
  cbm_write(CMD,cmd_parse,sizeof(cmd_parse));

  // Get display name
  cbm_write(CMD,display_name_query,sizeof(display_name_query));
  read_into_content();
  strncpy(display_name,content,22);

  // Get Created At
  cbm_write(CMD,created_at_query,sizeof(created_at_query));
  read_into_content();
  strncpy(created_at,content,22);

  // Get Content
  cbm_write(CMD,content_query,sizeof(content_query));
  read_into_content();

  // Close channel for Mastodon URL
  cbm_close(LFN);
  
  // Close command channel
  cbm_close(CMD);
}
