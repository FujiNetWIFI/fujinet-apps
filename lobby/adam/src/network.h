/**
 * #Fujinet Network Functions
 */

#include <stdbool.h>

#define NET_DEV 0x09
#define MODE_READ_WRITE 0x0C
#define NONE 0x00
#define ACK 0x80

#define MODE_PROTOCOL 0
#define MODE_JSON 1

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
unsigned char network_set_channel_mode(unsigned char mode);
unsigned char network_parse_json(void);
unsigned char network_json_query(const char *query);
