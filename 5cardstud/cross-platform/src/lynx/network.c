#include <lynx.h>
#include <tgi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "lynxfnio.h"


struct _oc
{
  unsigned char cmd;
  unsigned char mode;
  unsigned char trans;
  char url[128];
} OC; // open command data

typedef struct
{
  unsigned short rxBytesWaiting;
  unsigned char connected;
  unsigned char error;
} NetStatus;


unsigned char network_status(NetStatus *s)
{
    unsigned char r;
    unsigned short len;


    r = fnio_send(NET_DEV, "S", 1); 
    //if ((r & 0xF0) != NM_ACK)
    //    return(0);

    // Receive response
    r = fnio_recv(NET_DEV, (char *) s, &len);
    //if (((r & 0xF0) != NM_ACK) || (len == 0))
    //    return(0);

  return(1);
}


uint8_t getResponse(char *url, unsigned char *buffer, uint16_t max_length)
{
    unsigned short len;
    unsigned char r;
    unsigned int i;
    NetStatus ns;
    

    // Build packet to open URL
    OC.cmd = 'O';
    OC.mode = 12;
    OC.trans = 0;
    strncpy(OC.url, url, sizeof(OC.url));

    // Open the URL
    r = fnio_send(NET_DEV, (char *) &OC, sizeof(OC)); 
    //if ((r & 0xF0) != NM_ACK)
    //    return(0);

    //r = network_status(&ns);
    //if (!r) return(0);
  
    for(i=0; i<max_length; i+=256) {
        // Receive response
        r = fnio_recv(NET_DEV, (char *) &buffer[i], &len);
        //if (((r & 0xF0) != NM_ACK) || (len == 0))
        //    return(0);
        if (len==0) // break out if nothing more received
          break;
    }

    // Close the channel
    fnio_send(NET_DEV, (char *) "C", 1);
    return(1);
}
