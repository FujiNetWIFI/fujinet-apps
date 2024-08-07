#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "fujinet-fuji.h"
#include "edit_string.h"

#define TEST_CREATOR_ID 0x0100
#define TEST_APP_ID 0x1F
#define TEST_KEY_ID 0xCC

char write_buffer[65];
char read_buffer[65];

char *version = "v1.0.1";

int main(void)
{
	bool r;
	uint16_t read_count = 0;
	uint16_t write_count;

	clrscr();
	cprintf("FujiNet APPKEY Test %s\r\n", version);
	cputs("Enter a string to write to key:\r\n");

    cputcxy(0, 3, '>');
    cputcxy(21, 3, '<');
    edit_string(write_buffer, 64, 1, 3, 20, false);

    gotoxy(0,5);
	write_count = strlen(write_buffer);

	fuji_set_appkey_details(TEST_CREATOR_ID, TEST_APP_ID, DEFAULT);

	r = fuji_write_appkey(TEST_KEY_ID, write_count, (uint8_t *) &write_buffer[0]);;
	cprintf("WRITE size: %d r: %hd\r\n", write_count, r);

	r = fuji_read_appkey(TEST_KEY_ID, &read_count, (uint8_t *) &read_buffer[0]); // success is returned as true
	cprintf("READ r: %hd\r\n", r);
	cprintf("len=%u val='%s'\r\n", read_count, read_buffer);
	cputs("\r\nPress a key\r\n");
	cgetc();

	return 0;
}