
#include <c64.h>
#include <cbm.h>
#include <string.h>
#include <stdlib.h>

#define LFN 2     // The logical file number to use for I/O
#define DEV 12    // The network device #
#define SAN 2     // The secondary address (SA) to use on DEV.
#define CMD 15    // The secondary address for the Command channel

const char url[]="https://oldbytes.space/api/v1/timelines/public?limit=1";
const char cmd_parse[]="jsonparse,2";
const char display_name_query[]="jq,2,/0/account/display_name";
const char created_at_query[]="jq,2,/0/created_at";
const char content_query[]="jq,2,/0/content";
const char replies_query[]="jq,2,/0/replies_count";
const char reblogs_query[]="jq,2,/0/reblogs_count";
const char favorites_query[]="jq,2,/0/favorites_count";

static char tmp[800];

/**
 * @brief read data into tmp, cleaning it first.
 */
void read_into_tmp(void)
{
  memset(tmp,0,sizeof(tmp));
  cbm_read(LFN,tmp,sizeof(tmp));
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
void fetch_post(char *display_name,
		char *created_at,
		char *content,
		int *replies_count,
		int *reblogs_count,
		int *favorites_count)
{
  // Open command channel
  cbm_open(CMD,DEV,CMD,"");

  // Open channel for Mastodon URL
  cbm_open(LFN,DEV,SAN,url);

  // Ask FujiNet to parse incoming JSON
  cbm_write(CMD,cmd_parse,sizeof(cmd_parse));

  // Get display name
  cbm_write(CMD,display_name_query,sizeof(display_name_query));
  read_into_tmp();
  strncpy(display_name,tmp,40);

  // Get Created At
  cbm_write(CMD,created_at_query,sizeof(created_at_query));
  read_into_tmp();
  strncpy(created_at,tmp,40);

  // Get Content
  cbm_write(CMD,content_query,sizeof(content_query));
  read_into_tmp();
  strncpy(content,tmp,800);

  // Get replies count
  cbm_write(CMD,replies_query,sizeof(replies_query));
  read_into_tmp();
  *replies_count=atoi(tmp);

  // Get reblogs count
  cbm_write(CMD,reblogs_query,sizeof(reblogs_query));
  read_into_tmp();
  *reblogs_count=atoi(tmp);

  // Get favorites count
  cbm_write(CMD,favorites_query,sizeof(favorites_query));
  read_into_tmp();
  *favorites_count=atoi(tmp);
  
  // Close channel for Mastodon URL
  cbm_close(LFN);
  
  // Close command channel
  cbm_close(CMD);
}
