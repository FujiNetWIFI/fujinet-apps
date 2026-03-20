#include <lynx.h>
#include <tgi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "lynxfnio.h"


struct _cmd_pkt
{
  unsigned char cmd;
  unsigned char mode;
  unsigned char trans;
  char url[128];
} PKT; // packet for open and other commands

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

    PKT.cmd = FUJICMD_STATUS;
    r = fnio_send_buf(FUJI_DEVICEID_NETWORK, (char *) &PKT, 1); 
    if (!r)
      return(0);

    // Receive response
    r = fnio_recv_buf((char *) s, &len);
    if (!r || !len)
      return(0);

  return(1);
}


uint8_t getResponse(char *url, unsigned char *buffer, uint16_t max_length)
{
    unsigned short len;
    unsigned char r;
    uint16_t i;
    NetStatus ns;
    

    // Build packet to open URL (and reuse for commands)
    PKT.cmd = FUJICMD_OPEN;
    PKT.mode = 12;
    PKT.trans = 0;
    strncpy(PKT.url, url, sizeof(PKT.url));

    // Open the URL
    r = fnio_send_buf(FUJI_DEVICEID_NETWORK, (char *) &PKT, sizeof(PKT)); 
    if (!r)
     return(0);
    r = fnio_recv_ack();    // get the ACK or NAK of command completion
    if (!r)
     return(0);

    // get the bytes waiting
    r = network_status(&ns);
    if (!r)
      return(0);

    // read the data
    PKT.cmd = FUJICMD_READ;
    for(i=0; i<ns.rxBytesWaiting; i+=SERIAL_PACKET_SIZE) {
        r = fnio_send_buf(FUJI_DEVICEID_NETWORK, (char *) &PKT, 1);
        if (!r)
          return(0);
                
        // Receive response
        r = fnio_recv_buf((char *) &buffer[i], &len);
        if (len == 0 || r == 0) // break out if nothing more received, or error
          break;
    }

    // Close the channel
    PKT.cmd = FUJICMD_CLOSE;
    fnio_send_buf(FUJI_DEVICEID_NETWORK, (char *) &PKT, 1);
    fnio_recv_ack();
    return(1);
}
