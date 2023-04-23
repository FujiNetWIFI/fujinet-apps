/**
 * #Fujinet Network Functions
 */

#include <stdbool.h>

#define NET_DEV 0x09

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

unsigned char network_open(char *url, unsigned char mode, unsigned char translation);
unsigned char network_close(void);
unsigned char network_status(NetStatus *ns);
unsigned short network_read(char *buf, unsigned short len);
unsigned char network_write(unsigned char *buf, unsigned short len);

extern unsigned char response[1024];

#define NET_ERR_READY_TMOUT     1
#define NET_ERR_RTS_COM_ERR     2
#define NET_ERR_SEND_TMOUT      3
#define NET_ERR_SEND_COM_ERR    4
#define NET_ERR_SEND_DATA_BR    5
#define NET_ERR_READY_NAK       6
#define NET_ERR_READ_B_R        7
#define NET_ERR_DATA_NAK        8
#define NET_ERR_RECV_TMOUT      9
#define NET_ERR_RECV_BR        10
#define NET_ERR_RECV_DATA_BR   11
#define NET_ERR_RECV_NAK       12
#define NET_ERR_CLR_TMOUT      13
#define NET_ERR_CLR_ERR        14
#define NET_ERR_CLR_BR         15
#define NET_ERR_DATA_IN_ERROR  16
#define NET_ERR_DATAIN_TIMEOUT 17
#define NET_ERR_BAD_ASIG       18
#define NET_ERR_STAT_TMOUT     19
#define NET_ERR_STAT_BR        20
#define NET_ERR_STATUS_BR      21
#define NET_ERR_TCU_ERR1       22
#define NET_ERR_TCU_ERR2       23
#define NET_ERR_TRANS_TO1      24
#define NET_ERR_TRANS_TO2      25
#define NET_ERR_HAVE_ORFE      26
#define NET_ERR_TIMEOUT_ERR    28
#define NET_ERR_BAD_RDRF       29
#define NET_ERR_BAD_ORFE       30

#define NET_ERR_OK             0x80

unsigned char  network_open(char *url, unsigned char mode, unsigned char translation);
unsigned char  network_close(void);
unsigned char  network_status(NetStatus *ns);
unsigned short network_read(char *buf, unsigned short len);
unsigned char  network_write(unsigned char *buf, unsigned short len);

extern unsigned char response[1024];


