/**
 * Simple Mastodon Client
 * Shows public timeline of oldbytes.space
 *
 * JSON parsing example
 */

#include <apple2.h>
#include <conio.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "sp.h"

unsigned char net;
const char url[]="N:HTTPS://oldbytes.space/api/v1/timelines/public?limit=1";
const char display_name_query[]="/0/account/display_name";
const char created_at_query[]="/0/created_at";
const char content_query[]="/0/content";
unsigned short len;
unsigned long i;

void main(void)
{
  sp_init();
  net = sp_find_network();

  clrscr();

  printf("SHOWING PUBLIC POSTS FROM:\n");
  printf("OLDBYTES.SPACE\n\n");
  
  while (1)
    {
      // Open smartport device
      sp_open(net);

      // Do open URL
      sp_payload[0]=(strlen(url) & 0xFF) + 2;
      sp_payload[1]=(strlen(url) >> 8);
      sp_payload[2]=0x0C; // GET
      sp_payload[3]=0x80; // No translation
      memcpy(&sp_payload[4],url,strlen(url));
      sp_control(net,'O'); // Do open.

      // Set channel mode
      sp_payload[0]=0x01; // length of packet.
      sp_payload[1]=0x00;
      sp_payload[2]=0x01; // Set to JSON mode
      sp_control(net,0xFC); // Do it.

      // Parse JSON
      sp_control(net,'P'); // Do the parse

      // Display Name
      memset(sp_payload,0,sizeof(sp_payload));
      sp_payload[0]=strlen(display_name_query);
      sp_payload[1]=0;
      strcpy((char *)&sp_payload[2],display_name_query);
      sp_control(net,'Q'); // Query
      sp_status(net,'S'); // Get Status
      len=(unsigned short)sp_payload[0];
      memset(sp_payload,0,sizeof(sp_payload));
      sp_read(net,len); // Get Result
      revers(1);
      printf("%s\n",(const char *)&sp_payload[0]);

      // Display Created At
      memset(sp_payload,0,sizeof(sp_payload));
      sp_payload[0]=strlen(created_at_query);
      sp_payload[1]=0;
      strcpy((char *)&sp_payload[2],created_at_query);
      sp_control(net,'Q'); // Query
      sp_status(net,'S'); // Get Status
      len=(unsigned short)sp_payload[0];
      memset(sp_payload,0,sizeof(sp_payload));
      sp_read(net,len); // Get Result
      printf("%s\n",(const char *)&sp_payload[0]);
      revers(0);

      // Display content
      memset(sp_payload,0,sizeof(sp_payload));
      sp_payload[0]=strlen(content_query);
      sp_payload[1]=0;
      strcpy((char *)&sp_payload[2],content_query);
      sp_control(net,'Q'); // Query
      sp_status(net,'S'); // Get Status
      len=(unsigned short)sp_payload[0];
      memset(sp_payload,0,sizeof(sp_payload));
      sp_read(net,len); // Get Result
      printf("%s\n---\n",(const char *)&sp_payload[0]);
            
      // Close
      sp_close(net);

      for (i=0;i<262144;i++);
    }
}
