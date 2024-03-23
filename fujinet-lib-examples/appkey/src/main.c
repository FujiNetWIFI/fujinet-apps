#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "fujinet-fuji.h"

#define TEST_CREATOR_ID 0x0100
#define TEST_APP_ID 0x1F
#define TEST_KEY_ID 0xCC

static AppKeyDataBlock data;
char *test_data = "fujinet rocks";

unsigned char openkey(unsigned char open_mode)
{
    data.open.creator = TEST_CREATOR_ID;
    data.open.app = TEST_APP_ID;
    data.open.key = TEST_KEY_ID;
    data.open.mode = open_mode;
    data.open.reserved = 0x00;

    return fuji_appkey_open(&data.open);
}

int main(void)
{
    unsigned char r;

    cputs("\rFujiNet APPKEY Example\r\n");
    cputs("\r\nPress key to start...\r\n");
    cgetc();

    cputs("OPEN:  ");
    r = openkey(1);         // WRITE
    cprintf("%hd\r\n", r);

    cputs("WRITE: ");
    strcpy((char *) data.write.value, test_data);
    cprintf("(size: %d) ", strlen(test_data));
    r = fuji_appkey_write(strlen(test_data), &data.write);
    cprintf("%hd\r\n", r);

    cputs("OPEN:  ");
    r = openkey(0);         // READ
    cprintf("%hd\r\n", r);

    cputs("READ:  ");
    r = fuji_appkey_read(&data.read);
    cprintf("%hd\r\n", r);

    cprintf("len=%u val='%s'\r\n", data.read.length, data.read.value);
    cputs("Press a key");
    cgetc();

    return 0;
}