/**
 * ISS
 */

#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "map.h"
#include "iss.h"
#include "fujinet.h"

#define JSON 1
#define NET_DEV 0x09

const char url[]="N:HTTP://api.open-notify.org/iss-now.json";
const char longitude_query[]="Q/iss_position/longitude";
const char latitude_query[]="Q/iss_position/latitude";

struct _oc
{
  unsigned char cmd;
  unsigned char mode;
  unsigned char trans;
  char url[64];
} OC; // open command data

struct _cc
{
  unsigned char cmd;
  unsigned char chan;

} CC;

struct _scm
{
  unsigned char cmd;
  unsigned char mode;
} SCM; // set channel mode

char longitude[16];
char latitude[16];

char timestamp_str[11];
unsigned long timestamp;

void reload()
{
  unsigned short len;

  tgi_clear();

  OC.cmd = 'O'; // OPEN
  OC.mode = 12; // Read/write aka HTTP GET
  OC.trans = 0; // No translation
  strncpy(OC.url,url,sizeof(OC.url));

  fujinet_send(NET_DEV,(unsigned char *)&OC,sizeof(OC));

  sleep(1);
  
  SCM.cmd  = 0xFC; // Set channel mode
  SCM.mode = JSON; // to JSON mode
  
  fujinet_send(NET_DEV,(unsigned char *)&SCM,sizeof(SCM));

  sleep(1);

  fujinet_send(NET_DEV,(unsigned char *)"P",1); // Parse

  sleep(1);
  
  fujinet_send(NET_DEV,(unsigned char *)longitude_query,sizeof(longitude_query));
  fujinet_recv(NET_DEV,(unsigned char *)&longitude,&len);

  sleep(1);
  // DO IT AGAIN
  fujinet_send(NET_DEV,(unsigned char *)longitude_query,sizeof(longitude_query));
  fujinet_recv(NET_DEV,(unsigned char *)&longitude,&len);

  sleep(1);

  fujinet_send(NET_DEV,(unsigned char *)latitude_query,sizeof(latitude_query));
  fujinet_recv(NET_DEV,(unsigned char *)&latitude,&len);  

  CC.cmd = 'C'; // CLOSE
  CC.chan = 1;

  fujinet_send(NET_DEV,(unsigned char *)&CC,sizeof(CC));
}

void main(void)
{
  unsigned short len;
  
  tgi_install(tgi_static_stddrv);
  tgi_init();

  fujinet_init();
  
  tgi_setcolor(TGI_COLOR_WHITE);

  tgi_clear();

  OC.cmd = 'O'; // OPEN
  OC.mode = 12; // Read/write aka HTTP GET
  OC.trans = 0; // No translation
  strncpy(OC.url,url,sizeof(OC.url));

  fujinet_send(NET_DEV,(unsigned char *)&OC,sizeof(OC));

  tgi_outtext("O");
  
  sleep(1);
  
  SCM.cmd  = 0xFC; // Set channel mode
  SCM.mode = JSON; // to JSON mode
  
  fujinet_send(NET_DEV,(unsigned char *)&SCM,sizeof(SCM));

  tgi_outtext("S");

  sleep(1);

  fujinet_send(NET_DEV,(unsigned char *)"P",1); // Parse

  tgi_outtext("P");

  sleep(1);
  
  fujinet_send(NET_DEV,(unsigned char *)longitude_query,sizeof(longitude_query));
  fujinet_recv(NET_DEV,(unsigned char *)&longitude,&len);

  tgi_outtext("Q");

  sleep(1);
  // DO IT AGAIN
  fujinet_send(NET_DEV,(unsigned char *)longitude_query,sizeof(longitude_query));
  fujinet_recv(NET_DEV,(unsigned char *)&longitude,&len);

  tgi_outtext("Q");

  sleep(1);

  fujinet_send(NET_DEV,(unsigned char *)latitude_query,sizeof(latitude_query));
  fujinet_recv(NET_DEV,(unsigned char *)&latitude,&len);  

  tgi_outtext("Q");

  CC.cmd = 'C'; // CLOSE
  CC.chan = 1;

  fujinet_send(NET_DEV,(unsigned char *)&CC,sizeof(CC));
  
  while (1)
  {
    iss(atoi(longitude)+180,atoi(latitude)+270);
    map();
    sleep(15);
    reload();
  }
}
