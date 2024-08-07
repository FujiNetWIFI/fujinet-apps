#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "fujinet-network.h"

#include "mastodon.h"

uint8_t buffer[1024];
char url[] = "n:https://oldbytes.space/api/v1/timelines/public?limit=1";
char display_name_query[] = "/0/account/display_name";
char created_at_query[] = "/0/created_at";
char content_query[] = "/0/content";
char version[] = "v1.0.0";
uint8_t err = 0;

void main(void)
{
	int count = 0;
	setup();
	new_screen();
	while (1)
	{
		network_open(url, OPEN_MODE_READ, OPEN_TRANS_NONE);
		err = network_json_parse(url);
		handle_err("parse");

		count = network_json_query(url, display_name_query, (char *) buffer);
		if (count < 0) {
			err = -count;
			handle_err("query");
		}

		revers(1);
		printf("%40s", buffer);

		count = network_json_query(url, created_at_query, (char *) buffer);
		if (count < 0) {
			err = -count;
			handle_err("query");
		}
		printf("%40s", buffer);
		revers(0);

		count = network_json_query(url, content_query, (char *) buffer);
		if (count < 0) {
			err = -count;
			handle_err("query");
		}
		printf("%s\n---\n", buffer);

		network_close(url);

		pause(32000);
	}
}

void setup()
{
	uint8_t init_r = 0;
	init_r = network_init();
	bzero(buffer, 1024);

	if (init_r != 0)
	{
		err = init_r;
		handle_err("network_init failed");
	}
	printf("Press a key to continue.");
	cgetc();
}

void new_screen()
{
	clrscr();
	gotoxy(0, 0);
	printf("mastodon %s\n", version);
}

void handle_err(char *reason)
{
	if (err)
	{
		printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
		cgetc();
		exit(1);
	}
}

void pause(unsigned long time)
{
	unsigned long i;

	// TODO: perform platform specific pause
	for (i = 0; i < time; i++)
		;
}