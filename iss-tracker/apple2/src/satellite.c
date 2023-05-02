/**
 * ISS Tracker
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#define CENTER_X 4
#define CENTER_Y 4

#include <apple2.h>
#include <tgi.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include "satellite.h"
#include "sp.h"

extern unsigned char net; // Network device as found by sp_find_network()

const unsigned char satellite[8] = {
  0x20, /* ..X.|.... */
  0x50, /* .X.X|.... */
  0xA4, /* X.X.|.X.. */
  0x58, /* .X.X|X... */
  0x1A, /* ...X|X.X. */
  0x05, /* ....|.X.X */
  0x0A, /* ....|X.X. */
  0x04, /* ....|.X.. */
};

unsigned char x,y;
unsigned char backup[8][16];

// TODO: These tables are broken, recalculate them!

unsigned char xpos[360] = 
  { 0,0,1,2,3,3,4,5,6,7,7,8,9,10,10,11,12,13,14,14,15,16,17,17,18,19,20,21,21,22,23,24,24,25,26,27,28,28,29,30,31,31,32,33,34,35,35,36,37,38,38,39,40,41,42,42,43,44,45,45,46,47,48,49,49,50,51,52,52,53,54,55,56,56,57,58,59,59,60,61,62,63,63,64,65,66,66,67,68,69,70,70,71,72,73,73,74,75,76,77,77,78,79,80,80,81,82,83,84,84,85,86,87,87,88,89,90,91,91,92,93,94,94,95,96,97,98,98,99,100,101,101,102,103,104,105,105,106,107,108,108,109,110,111,112,112,113,114,115,115,116,117,118,119,119,120,121,122,122,123,124,125,126,126,127,128,129,129,130,131,132,133,133,134,135,136,136,137,138,139,140,140,141,142,143,143,144,145,146,147,147,148,149,150,150,151,152,153,154,154,155,156,157,157,158,159,160,161,161,162,163,164,164,165,166,167,168,168,169,170,171,171,172,173,174,175,175,176,177,178,178,179,180,181,182,182,183,184,185,185,186,187,188,189,189,190,191,192,192,193,194,195,196,196,197,198,199,199,200,201,202,203,203,204,205,206,206,207,208,209,210,210,211,212,213,213,214,215,216,217,217,218,219,220,220,221,222,223,224,224,225,226,227,227,228,229,230,231,231,232,233,234,234,235,236,237,238,238,239,240,241,241,242,243,244,245,245,246,247,248,248,249,250,251,252,252,253,254,255,255,0,1,2,3,3,4,5,6,6,7,8,9,10,10,11,12,13,13,14,15,16,17,17,18,19,20,20,21,22,23,  };

unsigned char ypos[360] = 
  { 0,0,0,1,1,2,2,3,3,4,4,4,5,5,6,6,7,7,8,8,8,9,9,10,10,11,11,12,12,12,13,13,14,14,15,15,16,16,16,17,17,18,18,19,19,20,20,20,21,21,22,22,23,23,24,24,24,25,25,26,26,27,27,28,28,28,29,29,30,30,31,31,32,32,32,33,33,34,34,35,35,36,36,36,37,37,38,38,39,39,40,40,40,41,41,42,42,43,43,44,44,44,45,45,46,46,47,47,48,48,48,49,49,50,50,51,51,52,52,52,53,53,54,54,55,55,56,56,56,57,57,58,58,59,59,60,60,60,61,61,62,62,63,63,64,64,64,65,65,66,66,67,67,68,68,68,69,69,70,70,71,71,72,72,72,73,73,74,74,75,75,76,76,76,77,77,78,78,79,79,80,80,80,81,81,82,82,83,83,84,84,84,85,85,86,86,87,87,88,88,88,89,89,90,90,91,91,92,92,92,93,93,94,94,95,95,96,96,96,97,97,98,98,99,99,100,100,100,101,101,102,102,103,103,104,104,104,105,105,106,106,107,107,108,108,108,109,109,110,110,111,111,112,112,112,113,113,114,114,115,115,116,116,116,117,117,118,118,119,119,120,120,120,121,121,122,122,123,123,124,124,124,125,125,126,126,127,127,128,128,128,129,129,130,130,131,131,132,132,132,133,133,134,134,135,135,136,136,136,137,137,138,138,139,139,140,140,140,141,141,142,142,143,143,144,144,144,145,145,146,146,147,147,148,148,148,149,149,150,150,151,151,152,152,152,153,153,154,154,155,155,156,156,156,157,157,158,158,159,159,  };

const char url[]="N:HTTP://api.open-notify.org/iss-now.json";
const char longitude_query[]="/iss_position/longitude";
const char latitude_query[]="/iss_position/latitude";
const char timestamp_query[]="/timestamp";

bool satellite_fetch(int *lon, int *lat, char *lon_s, char *lat_s, unsigned long *ts)
{
  unsigned short len;
  
  memset(lon_s,0,16);
  memset(lat_s,0,16);
  
  // Open
  sp_open(net);
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

  // Parse the JSON
  sp_control(net,'P'); // Do the parse

  // Query for timestamp
  memset(sp_payload,0,sizeof(sp_payload));
  sp_payload[0]=10;
  sp_payload[1]=0;
  strncpy(&sp_payload[2],timestamp_query,10);
  sp_control(net,'Q'); // Query
  sp_status(net,'S'); // Get Status
  len=(unsigned short)sp_payload[0];
  memset(sp_payload,0,sizeof(sp_payload));
  sp_read(net,len); // Get Result
  *ts=atol((const char *)&sp_payload[0]);
  
  // Query for Longitude
  sp_payload[0]=24;
  sp_payload[1]=0;
  memcpy(&sp_payload[2],longitude_query,24);
  sp_control(net,'Q'); // Query
  sp_status(net,'S'); // Get status
  len=(unsigned short)sp_payload[0]; // Get # of bytes waiting.
  memset(sp_payload,0,sizeof(sp_payload));
  sp_read(net,len); // Get result
  memcpy(lon_s,sp_payload,len);
  *lon=atoi(lon_s);
  
  // Query for Latitude
  sp_payload[0]=23;
  sp_payload[1]=0;
  memcpy(&sp_payload[2],latitude_query,23);
  sp_control(net,'Q'); // Query
  sp_status(net,'S'); // Get status
  len=(unsigned short)sp_payload[0]; // Get # of bytes waiting.
  memset(sp_payload,0,sizeof(sp_payload));
  sp_read(net,len); // Get result
  memcpy(lat_s,sp_payload,len);
  *lat=atoi(lat_s);

  sp_close(net);

  return true; // todo come back here and add error handling.
}

void satellite_draw(int lon, int lat)
{
  unsigned char i,j;
  signed char b;
  x=xpos[lon+180]-CENTER_X;
  y=ypos[lat+180]-CENTER_Y;
  
  for (i=0;i<8;i++)
    {
      b=satellite[i];
      for (j=0;j<16;j++)
        {
          backup[i][j]=tgi_getpixel(x+j,y+i);
          if (b < 0)
            tgi_setcolor(TGI_COLOR_WHITE2);
          else
            tgi_setcolor(TGI_COLOR_BLACK2);
          tgi_setpixel(x+j,y+i);
          if (j & 1)
            b <<= 1;
        }
    }
}

void satellite_erase(void)
{
  unsigned char i,j;
  
  for (i=0;i<8;i++)
    {
      for (j=0;j<16;j++)
        {
          tgi_setcolor(backup[i][j]);
          tgi_setpixel(x+j,y+i);
        }
    }
}
