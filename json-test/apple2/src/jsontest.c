/**
 * jsontest.c
 *
 * A quick test by Norman Davie
 */

#include <apple2.h>
#include <peekpoke.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sp.h"

const char url[] = "N:HTTP://api.open-notify.org/iss-now.json";
const char longitude_query[]  = "/iss_position/longitude";
const char latitude_query[]   = "/iss_position/latitude";
const char timestamp_query[]  = "/TIMESTAMP";

void main()
{
  char timestamp[30];
  int8_t net,err;
  unsigned short len;

  sp_init();
  net = sp_find_network();

  printf("network is %d\n", net);
  // Open
  sp_open(net);



  sp_payload[0] = (strlen(url) & 0xFF) + 2;
  sp_payload[1] = (strlen(url) >> 8);
  sp_payload[2] = 0x0C; // GET
  sp_payload[3] = 0x80; // No translation
  memcpy(&sp_payload[4], url, strlen(url));

  printf("opening ");
  printf(url);
 
  err = sp_control(net, 'O'); // Do open.
  printf(" err:%d\n",err);

  printf("setting channel mode to JSON\n");

  // Set channel mode
  sp_payload[0] = 0x01; // length of packet.
  sp_payload[1] = 0x00;
  sp_payload[2] = 0x01;  // Set to JSON mode
  err = sp_control(net, 0xFC); // Do it.
  printf(" err:%d\n", err);

  printf("parse the JSON");

  // Parse the JSON
  err=sp_control(net, 'P'); // Do the parse
  printf(" err:%d\n", err);

 

  // Query for timestamp
  memset(sp_payload, 0, sizeof(sp_payload));
  sp_payload[0] = 10;
  sp_payload[1] = 0;
  //"/timestamp"
  // 1234567890

  len = sizeof(timestamp_query)-1;
  printf("query '%s'\n",timestamp_query);
  printf("query length is %d bytes\n", len);
  strncpy((char *)&sp_payload[2], timestamp_query, len);

  err=sp_control(net, 'Q'); // Query
  printf(" err:%d\n", err);

  printf("get status");

  err=sp_status(net, 'S');  // Get Status
  len = (unsigned short)sp_payload[0];
  printf(" err:%d\n", err);

  printf("%d bytes waiting\n", len);

  memset(sp_payload, 0, sizeof(sp_payload));

  printf("reading...");

  err=sp_read(net, len); // Get Result
  printf(" err:%d\n", err);

  printf("NULLing result...\n");

  sp_payload[len+1] = '\0';
  strncpy(timestamp, (char *)&sp_payload[2], sizeof(timestamp));

  printf("Result is '%s'\n", timestamp);

  sp_close(net);

  gets(timestamp);
}