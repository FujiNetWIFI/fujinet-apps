/**
 * #Fujinet Network Functions
 */

#include <stdbool.h>

#define NET_DEV 0x09
#define FUJI_DEV 0x0F

#define MAX_URL      (256)
#define MAX_QUERY    (128)
#define MAX_APP_DATA (128)


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

typedef struct
{
    unsigned char cmd;
    unsigned short mode;
    unsigned char trans;
    unsigned char url[MAX_URL];
} FUJI_CMD;

typedef struct 
{
  unsigned char cmd;
           char mode
} FUJI_SET_CHANNEL;


typedef struct 
{
  unsigned char cmd;
  char query[MAX_QUERY];
} FUJI_JSON_QUERY;

typedef struct
{
    unsigned char cmd;
    unsigned short creator;
    unsigned char app;
    unsigned char key;
} FUJI_APP;

 typedef struct
{
    unsigned char cmd;
    unsigned short creator;
    unsigned char app;
    unsigned char key;
    char data[MAX_APP_DATA];
} FUJI_APP_DATA;

unsigned char network_open(char* url, unsigned char mode, unsigned char translation);
unsigned char network_close(void);
unsigned char network_status(NetStatus *ns);
unsigned short network_read(char *buf, unsigned short len);
unsigned char network_write(unsigned char *buf, unsigned short len);
unsigned char network_set_channel_mode(unsigned char mode);
unsigned char network_parse_json(void);
unsigned char network_json_query(const char *query);
