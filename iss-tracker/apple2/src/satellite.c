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

/* AWK script to calculate the longitude and latitude values:

awk 'BEGIN{
  x=1;
  MAXX=280;
  MAXY=160;
  printf("unsigned char longitude[360] =\n{\n");
  for(i=0;i<360;i++) {
    printf("%d,",int(i*MAXX/360));
    if (x%10 == 0) { printf("\n"); }
    x++;
  }
  printf("};\n\nunsigned char latitude[180] =\n{\n");
  for(i=179;i>=0;i--) {
    printf("%d,",int(i*MAXY/180));
    if (x%10 == 0) { printf("\n"); }
    x++;
  }
  printf("};\n");
}'

*/
unsigned char longitude[360] =
{
0,0,1,2,3,3,4,5,6,7,
7,8,9,10,10,11,12,13,14,14,
15,16,17,17,18,19,20,21,21,22,
23,24,24,25,26,27,28,28,29,30,
31,31,32,33,34,35,35,36,37,38,
38,39,40,41,42,42,43,44,45,45,
46,47,48,49,49,50,51,52,52,53,
54,55,56,56,57,58,59,59,60,61,
62,63,63,64,65,66,66,67,68,69,
70,70,71,72,73,73,74,75,76,77,
77,78,79,80,80,81,82,83,84,84,
85,86,87,87,88,89,90,91,91,92,
93,94,94,95,96,97,98,98,99,100,
101,101,102,103,104,105,105,106,107,108,
108,109,110,111,112,112,113,114,115,115,
116,117,118,119,119,120,121,122,122,123,
124,125,126,126,127,128,129,129,130,131,
132,133,133,134,135,136,136,137,138,139,
140,140,141,142,143,143,144,145,146,147,
147,148,149,150,150,151,152,153,154,154,
155,156,157,157,158,159,160,161,161,162,
163,164,164,165,166,167,168,168,169,170,
171,171,172,173,174,175,175,176,177,178,
178,179,180,181,182,182,183,184,185,185,
186,187,188,189,189,190,191,192,192,193,
194,195,196,196,197,198,199,199,200,201,
202,203,203,204,205,206,206,207,208,209,
210,210,211,212,213,213,214,215,216,217,
217,218,219,220,220,221,222,223,224,224,
225,226,227,227,228,229,230,231,231,232,
233,234,234,235,236,237,238,238,239,240,
241,241,242,243,244,245,245,246,247,248,
248,249,250,251,252,252,253,254,255,255,
256,257,258,259,259,260,261,262,262,263,
264,265,266,266,267,268,269,269,270,271,
272,273,273,274,275,276,276,277,278,279
};

unsigned char latitude[180] =
{
159,158,157,156,155,154,153,152,152,151,
150,149,148,147,146,145,144,144,143,142,
141,140,139,138,137,136,136,135,134,133,
132,131,130,129,128,128,127,126,125,124,
123,122,121,120,120,119,118,117,116,115,
114,113,112,112,111,110,109,108,107,106,
105,104,104,103,102,101,100,99,98,97,
96,96,95,94,93,92,91,90,89,88,
88,87,86,85,84,83,82,81,80,80,
79,78,77,76,75,74,73,72,72,71,
70,69,68,67,66,65,64,64,63,62,
61,60,59,58,57,56,56,55,54,53,
52,51,50,49,48,48,47,46,45,44,
43,42,41,40,40,39,38,37,36,35,
34,33,32,32,31,30,29,28,27,26,
25,24,24,23,22,21,20,19,18,17,
16,16,15,14,13,12,11,10,9,8,
8,7,6,5,4,3,2,1,0,0
};

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
  x=longitude[lon+180]-CENTER_X;
  y=latitude[lat+90]-CENTER_Y;
  
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
