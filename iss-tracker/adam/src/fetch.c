/** 
 * iss-tracker
 *
 * based on Bill Kendrick's Atari version
 *
 * @author thomas cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details
 */

#include <stdlib.h>
#include <eos.h>
#include <smartkeys.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <video/tms99x8.h>
#include <graphics.h>
#include "fetch.h"
#include "input.h"

#define URL "N:HTTP://api.open-notify.org/iss-now.json"
#define TRACKING_URL "N:HTTP://api.wheretheiss.at/v1/satellites/25544/positions?timestamps=%ld,%ld"
#define WHO_URL "N:HTTP://api.open-notify.org/astros.json"
#define ACK 0x80
#define NACK 0x8C
#define JSON 1
#define NET 0x09
#define Y_CENTER 80
#define X_CENTER 128
/* We fetch 10 x 2 positions per request = 20 positions.
   At 5 minutes, we get 20 * 5 = 100 minutes, or approx. 1.5 hrs
   of future positions (approx. one orbit!) */
#define TRACK_TIMESKIP (5 /* minutes */ * 60 /* seconds per minute */)


const char longitude_query[]="Q/iss_position/longitude";
const char latitude_query[]="Q/iss_position/latitude";
const char timestamp_query[]="Q/timestamp";

const char longitude0_tracking_query[]="Q/0/longitude";
const char longitude1_tracking_query[]="Q/1/longitude";
const char latitude0_tracking_query[]="Q/0/latitude";
const char latitude1_tracking_query[]="Q/1/latitude";

struct _oc
{
  unsigned char cmd;
  unsigned char mode;
  unsigned char trans;
  unsigned char url[256];
} OC; // open command data

struct _scm
{
  unsigned char cmd;
  unsigned char mode;
} SCM; // set channel mode

char longitude[16];
char latitude[16];

char timestamp_str[11];
unsigned long timestamp;

const unsigned char xpos[360] = 
  { 0,0,1,2,2,3,4,4,5,6,7,7,8,9,9,10,11,12,12,13,14,14,15,16,17,17,18,19,19,20,21,22,22,23,24,24,25,26,27,27,28,29,29,30,31,32,32,33,34,34,35,36,36,37,38,39,39,40,41,41,42,43,44,44,45,46,46,47,48,49,49,50,51,51,52,53,54,54,55,56,56,57,58,59,59,60,61,61,62,63,64,64,65,66,66,67,68,68,69,70,71,71,72,73,73,74,75,76,76,77,78,78,79,80,81,81,82,83,83,84,85,86,86,87,88,88,89,90,91,91,92,93,93,94,95,96,96,97,98,98,99,100,100,101,102,103,103,104,105,105,106,107,108,108,109,110,110,111,112,113,113,114,115,115,116,117,118,118,119,120,120,121,122,123,123,124,125,125,126,127,128,128,129,130,130,131,132,132,133,134,135,135,136,137,137,138,139,140,140,141,142,142,143,144,145,145,146,147,147,148,149,150,150,151,152,152,153,154,155,155,156,157,157,158,159,160,160,161,162,162,163,164,164,165,166,167,167,168,169,169,170,171,172,172,173,174,174,175,176,177,177,178,179,179,180,181,182,182,183,184,184,185,186,187,187,188,189,189,190,191,192,192,193,194,194,195,196,196,197,198,199,199,200,201,201,202,203,204,204,205,206,206,207,208,209,209,210,211,211,212,213,214,214,215,216,216,217,218,219,219,220,221,221,222,223,224,224,225,226,226,227,228,228,229,230,231,231,232,233,233,234,235,236,236,237,238,238,239,240,241,241,242,243,243,244,245,246,246,247,248,248,249,250,251,251,252,253,253,254,255 };

const unsigned char ypos[360] = 
  { 160,159,159,158,158,157,157,156,156,156,155,155,154,154,153,153,152,152,152,151,151,150,150,149,149,148,148,148,147,147,146,146,145,145,144,144,144,143,143,142,142,141,141,140,140,140,139,139,138,138,137,137,136,136,136,135,135,134,134,133,133,132,132,132,131,131,130,130,129,129,128,128,128,127,127,126,126,125,125,124,124,124,123,123,122,122,121,121,120,120,120,119,119,118,118,117,117,116,116,116,115,115,114,114,113,113,112,112,112,111,111,110,110,109,109,108,108,108,107,107,106,106,105,105,104,104,104,103,103,102,102,101,101,100,100,100,99,99,98,98,97,97,96,96,96,95,95,94,94,93,93,92,92,92,91,91,90,90,89,89,88,88,88,87,87,86,86,85,85,84,84,84,83,83,82,82,81,81,80,80,80,79,79,78,78,77,77,76,76,76,75,75,74,74,73,73,72,72,72,71,71,70,70,69,69,68,68,68,67,67,66,66,65,65,64,64,64,63,63,62,62,61,61,60,60,60,59,59,58,58,57,57,56,56,56,55,55,54,54,53,53,52,52,52,51,51,50,50,49,49,48,48,48,47,47,46,46,45,45,44,44,44,43,43,42,42,41,41,40,40,40,39,39,38,38,37,37,36,36,36,35,35,34,34,33,33,32,32,32,31,31,30,30,29,29,28,28,28,27,27,26,26,25,25,24,24,24,23,23,22,22,21,21,20,20,20,19,19,18,18,17,17,16,16,16,15,15,14,14,13,13,12,12,12,11,11,10,10,9,9,8,8,8,7,7,6,6,5,5,4,4,4,3,3,2,2,1,1,0,0 };

State fetch(void)
{
  DCB *dcb;
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("   FETCHING DATA.");
  
  // Perform open

  OC.cmd = 'O'; // OPEN
  OC.mode = 12; // Read/write aka HTTP GET
  OC.trans = 0; // No translation
  strncpy(OC.url,URL,sizeof(OC.url));

  if (eos_write_character_device(NET,(unsigned char *)OC,sizeof(OC)) != ACK)
    {
      smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
      smartkeys_status("   COULD NOT FETCH DATA. PROGRAM HALTED.");
      exit(1);
    }

  // Set channel mode to JSON

  SCM.cmd  = 0xFC; // Set channel mode
  SCM.mode = JSON; // to JSON mode

  if (eos_write_character_device(NET,(unsigned char *)SCM,sizeof(SCM)) != ACK)
    {
      smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
      smartkeys_status("   COULD NOT SET CHANNEL MODE TO JSON. PROGRAM HALTED.");
      exit(1);
    }
    
  // Ask #FujiNet to parse the JSON

  if (eos_write_character_device(NET,"P",1) != ACK)
    {
      smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
      smartkeys_status("   COULD NOT PARSE JSON. PROGRAM HALTED.");
      exit(1);
    }

  // Wait for device to parse, then Query for all the information
  while (eos_request_device_status(NET,dcb) != ACK);
  
  eos_write_character_device(NET,timestamp_query,sizeof(timestamp_query));
  while (eos_read_character_device(NET,timestamp_str,1024) == NACK);
  timestamp=atol(timestamp_str);
  
  eos_write_character_device(NET,longitude_query,sizeof(longitude_query));
  while (eos_read_character_device(NET,longitude,1024) == NACK);

  eos_write_character_device(NET,latitude_query,sizeof(latitude_query));
  while (eos_read_character_device(NET,latitude,1024) == NACK);

  // Close the connection

  eos_write_character_device(NET,"C",1);

  return DISPLAY;
}

unsigned char fetch_latitude_to_y(void)
{
  int lat = atoi(latitude)+180;

  return ypos[lat];
}

unsigned char fetch_longitude_to_x(void)
{
  int lon = atoi(longitude)+180;

  return xpos[lon];
}

void fetch_track()
{
  DCB *dcb;
  
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("   FETCHING UPCOMING ISS POSITIONS...");

  vdp_color(1,1,4); // using color clash in our favor!
  
  for (int i=0;i<10;i++)
    {
      char tmp[20];
      int lat, lon;
      unsigned char tx,ty;
      
      sprintf(OC.url,
	      TRACKING_URL,
	      timestamp + TRACK_TIMESKIP * (i * 2),
	      timestamp + TRACK_TIMESKIP * ((i * 2) + 1));

      eos_write_character_device(NET,(unsigned char *)OC,sizeof(OC));
      eos_write_character_device(NET,(unsigned char *)SCM,sizeof(SCM));
      eos_write_character_device(NET,"P",1);

      // wait for parse to complete
      while(eos_request_device_status(NET,dcb) != 0x80);
      
      eos_write_character_device(NET,latitude0_tracking_query,sizeof(latitude0_tracking_query));
      while (eos_read_character_device(NET,tmp,1024) != 0x80);
      lat = atoi(tmp)+180;
      ty = ypos[lat];

      eos_write_character_device(NET,longitude0_tracking_query,sizeof(longitude0_tracking_query));
      while (eos_read_character_device(NET,tmp,1024) != 0x80);
      lon = atoi(tmp)+180;
      tx = xpos[lon]-24;

      plot(tx,ty);

      eos_write_character_device(NET,latitude1_tracking_query,sizeof(latitude1_tracking_query));
      while (eos_read_character_device(NET,tmp,1024) != 0x80);
      lat = atoi(tmp)+180;
      ty = ypos[lat];

      eos_write_character_device(NET,longitude1_tracking_query,sizeof(longitude1_tracking_query));
      while (eos_read_character_device(NET,tmp,1024) != 0x80);
      lon = atoi(tmp)+180;
      tx = xpos[lon]-24;

      plot(tx,ty);

      eos_write_character_device(NET,"C",1);
      csleep(100);
    }
  return DISPLAY;
}

void fetch_who()
{
  char name[64];
  const char qf[]="Q/people/%d/name";
  char q[20];
  int i=0;
  char statusmsg[128];
  char k=0;
  DCB *dcb;
  
  strcpy(OC.url,WHO_URL);
  eos_write_character_device(NET,(unsigned char *)OC,sizeof(OC));
  eos_write_character_device(NET,(unsigned char *)SCM,sizeof(SCM));
  eos_write_character_device(NET,"P",1);

  // wait for parse to complete
  while(eos_request_device_status(NET,dcb) != 0x80);

  memset(name,0,64);
  
  while(1)
    {
    who_next:
      memset(name,0,64);
      
      sprintf(q,qf,i++);
      eos_write_character_device(NET,q,sizeof(q));
      while (eos_read_character_device(NET,name,1024) == NACK);

      if (name[0]==0x00)
	goto who_exit;
      
      sprintf(statusmsg,"   ASTRONAUT: %s",name);
      smartkeys_display(NULL,NULL,NULL,NULL," ABORT","  NEXT");
      smartkeys_status(statusmsg);
      
      while (k != KEY_SMART_V || k != KEY_SMART_VI)
	{
	  k=input();
	  switch(k)
	    {
	    case KEY_SMART_V:
	      goto who_exit;
	    case KEY_SMART_VI:
	      k=0;
	      goto who_next;
	    }
	}     
    }
  
 who_exit:
  eos_write_character_device(NET,"C",1);

  return FETCH;
}
