/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose json functions
 */

#include <cbm.h>
#include <string.h>
#include "defines.h"

static const char cmd_parse[]="jsonparse,2";
static const char cmd_query[]="jq,2,";

void json_init(void)
{
  cbm_open(CMD,DEV,CMD,"");
}

void json_parse(void)
{
  cbm_write(CMD,cmd_parse,sizeof(cmd_parse));
}

void json_query(char *s)
{
  char query[40];

  strcpy(query,cmd_query);
  strcat(query,s);
  cbm_write(CMD,query,sizeof(query));
}

void json_done(void)
{
  cbm_close(CMD);
}
