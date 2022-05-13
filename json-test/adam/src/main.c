/**
 * json-test
 *
 * Shows how to use the #FujiNet JSON parsing functions
 *
 * @author Thomas Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <eos.h>

#define NET_DEV 0x09
#define CHANNEL_MODE_JSON 0x01
#define ACK 0x80

// const char *url = "N:HTTPS://jsonplaceholder.typicode.com/posts/1";

const char *url = "N:HTTPS://oldbytes.space/api/v1/timelines/public?limit=1";

char response[1024];

void main(void)
{
  unsigned char r;
  
  struct _oc
  {
    unsigned char cmd;
    char mode;
    char trans;
    char url[256];
  } OC; // Open command

  struct _scm
  {
    unsigned char cmd;
    char mode;
  } SCM; // Set Channel Mode command

  struct _qc
  {
    unsigned char cmd;
    char query[128];
  } QC; // Query command
  
  OC.cmd = 'O';
  OC.mode = 12;
  OC.trans = 3;
  strncpy(OC.url,url,256);

  SCM.cmd = 0xFC;
  SCM.mode = CHANNEL_MODE_JSON;

  QC.cmd = 'Q';
  
  printf("JSON TEST PROGRAM\n\n");

  printf("OPENING TEST URL:\n%s\n",url);

  if (eos_write_character_device(NET_DEV,(unsigned char *)OC,sizeof(OC)) != ACK)
    {
      printf("ERROR.\n");
      return;
    }
  else
    printf("OPENED.\n");
  
  printf("SETTING CHANNEL MODE TO JSON\n");
  if (eos_write_character_device(NET_DEV,(unsigned char *)SCM,sizeof(SCM)) != ACK)
    {
      printf("ERROR.\n");
      return;
    }
  else
    printf("CHANGED.\n");
  
  printf("PARSING JSON.\n");
  if (eos_write_character_device(NET_DEV,"P",1) != ACK)
    {
      printf("ERROR.\n");
      return;
    }

  printf("QUERYING FOR DATA\n");

  strncpy(QC.query,"/0/account/display_name",128);
  while (eos_write_character_device(NET_DEV,(unsigned char *)QC,sizeof(QC)) < 0x80);
  while ((r = eos_read_character_device(NET_DEV,response,1024)) < 0x80);
  printf("\nNAME: %s\n\n",response);

  strncpy(QC.query,"/0/content",128);
  while (eos_write_character_device(NET_DEV,(unsigned char *)QC,sizeof(QC)) < 0x80);
  while ((r = eos_read_character_device(NET_DEV,response,1024)) < 0x80);
  printf("\n%s\n\n",response);
    
  printf("CLOSING.\n");
  eos_write_character_device(NET_DEV,"C",1);  
}
