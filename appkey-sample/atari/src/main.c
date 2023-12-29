#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <atari.h>
#include "system.h"
#include "fn_io.h"

#define TEST_CREATOR_ID 0x0100
#define TEST_APP_ID 0x1F
#define TEST_KEY_ID 0xCC

#define TEST_DATA "Hello world?"

static AppKeyDataBlock data;

unsigned char sio_readkey()
{
    return fn_io_appkey_read(&data.read);
}

unsigned char sio_writekey()
{
    strcpy((char *) data.write.value, TEST_DATA);
    return fn_io_appkey_write(sizeof(TEST_DATA), &data.write);
}

unsigned char sio_openkey(unsigned char open_mode)
{
    data.open.creator = TEST_CREATOR_ID;
    data.open.app = TEST_APP_ID;
    data.open.key = TEST_KEY_ID;
    data.open.mode = open_mode;
    data.open.reserved = 0x00;

    return fn_io_appkey_open(&data.open);
}
int main(void)
{
    unsigned char r;

    cputs("\rPress key to start?\r\n\n");
    cgetc();

    // Execute an OPEN command before accessing the key
    // We set 'mode' to 1 indicating a write operation
    cputs("OPEN:  ");
    r = sio_openkey(1);
    cprintf("%hd\r\n", r);

    // Now write some data to the key
    cputs("WRITE: ");
    r = sio_writekey();
    cprintf("%hd\r\n", r);

    // Open the key again before performing a new operation
    // We set 'mode' to 0 indicating a read operation
    cputs("OPEN:  ");
    r = sio_openkey(0);
    cprintf("%hd\r\n", r);

    // Now read the data in the key
    cputs("READ:  ");
    r = sio_readkey();
    cprintf("%hd\r\n", r);

    // Write out the results
    cprintf("len=%u val='%s'\r\n", data.read.length, data.read.value);

    cputs("Press key to reboot!");
    cgetc();
    cold_start();

    return 0;
}