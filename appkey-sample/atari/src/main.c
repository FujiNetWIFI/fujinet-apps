#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <atari.h>
#include "sio.h"

#define SIO_DEVICEID 0x70
#define SIO_UNITID 0x01
#define SIO_TIMEOUT 0x09 // (64/60)-second unit values

#define SIO_XFER_NONE 0x00
#define SIO_XFER_READ 0x40
#define SIO_XFER_WRITE 0x80

#define SIO_CMD_APPKEY_WRITE 0xDE
#define SIO_CMD_APPKEY_READ 0xDD
#define SIO_CMD_APPKEY_OPEN 0xDC
#define SIO_CMD_APPKEY_CLOSE 0xDB

#define MAX_APPKEY_LEN 64

#define TEST_CREATOR_ID 0x0100
#define TEST_APP_ID 0x1F
#define TEST_KEY_ID 0xCC

#define TEST_DATA "Hello world?"

typedef union {
    struct
    {
        unsigned int creator;
        unsigned char app;
        unsigned char key;
        unsigned char mode;
        unsigned char reserved;
    } open;
    struct
    {
        unsigned int length;
        unsigned char value[MAX_APPKEY_LEN];
        
    } read;
    struct
    {
        unsigned char value[MAX_APPKEY_LEN];
    } write;
} datablock;

static datablock data;

unsigned char sio_readkey()
{
    OS.dcb.ddevic = SIO_DEVICEID;
    OS.dcb.dunit = SIO_UNITID;
    OS.dcb.dtimlo = SIO_TIMEOUT;

    OS.dcb.dcomnd = SIO_CMD_APPKEY_READ;
    OS.dcb.dstats = SIO_XFER_READ;
    OS.dcb.dbuf = &data;
    OS.dcb.dbyt = sizeof(data.read);
    OS.dcb.daux = 0;

    siov();

    return OS.dcb.dstats;
}

unsigned char sio_writekey()
{
    strcpy(data.write.value, TEST_DATA);

    OS.dcb.ddevic = SIO_DEVICEID;
    OS.dcb.dunit = SIO_UNITID;
    OS.dcb.dtimlo = SIO_TIMEOUT;

    OS.dcb.dcomnd = SIO_CMD_APPKEY_WRITE;
    OS.dcb.dstats = SIO_XFER_WRITE;
    OS.dcb.dbuf = &data.write;
    OS.dcb.dbyt = sizeof(data.write);
    /* Note that we set DAUX (DAUX1+DAUX2) to the number of valid bytes in the buffer
        even though we're actually sending the full MAX_APPKEY_LEN number of bytes total.
    */
    OS.dcb.daux = sizeof(TEST_DATA);

    siov();

    return OS.dcb.dstats;
}

unsigned char sio_openkey(unsigned char open_mode)
{
    data.open.creator = TEST_CREATOR_ID;
    data.open.app = TEST_APP_ID;
    data.open.key = TEST_KEY_ID;
    data.open.mode = open_mode;
    data.open.reserved = 0x00;

    OS.dcb.ddevic = SIO_DEVICEID;
    OS.dcb.dunit = SIO_UNITID;
    OS.dcb.dtimlo = SIO_TIMEOUT;

    OS.dcb.dcomnd = SIO_CMD_APPKEY_OPEN;
    OS.dcb.dstats = SIO_XFER_WRITE;
    OS.dcb.dbuf = &data;
    OS.dcb.dbyt = sizeof(data.open);
    OS.dcb.daux = 0;

    siov();

    return OS.dcb.dstats;
}
int main(void)
{
    unsigned char r;

    cputs("\rPress key to start?\r\n\n");
    cgetc();

    // Execute an OPEN command before accessing the key
    // We set 'mode' to 1 indicating a write operation
    cputs("OPEN: ");
    r = sio_openkey(1);
    cprintf("%hd\r\n", r);

    // Now write some data to the key
    cputs("WRITE: ");
    r = sio_writekey();
    cprintf("%hd\r\n", r);

    // Open the key again before performing a new operation
    // We set 'mode' to 0 indicating a read operation    
    cputs("OPEN: ");
    r = sio_openkey(0);
    cprintf("%hd\r\n", r);

    // Now read the data in the key
    cputs("READ:");
    r = sio_readkey();
    cprintf("%hd\r\n", r);

    // Write out the results
    cprintf("len=%u val='%s'\r\n\n", data.read.length, data.read.value);

    cputs("Press key to reboot?");
    cgetc();
    cold_start();

    return 0;
}
