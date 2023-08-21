/**
 * #Fujinet Network Functions
 */

#include <stdbool.h>

#define NET_DEV 0x09

typedef struct
{
  unsigned short rxBytesWaiting;
  bool connected;
  unsigned char error;
} NetStatus;

unsigned char network_open(char* url, unsigned char mode, unsigned char translation);
unsigned char network_close(void);
unsigned char network_status(NetStatus *ns);
unsigned short network_read(char *buf, unsigned short len);
unsigned char network_write(unsigned char *buf, unsigned short len);
