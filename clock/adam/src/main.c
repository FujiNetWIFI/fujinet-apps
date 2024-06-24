/*
 * Clock
 *
 * @license gpl v. 3, see LICENSE.md for details.
 */

#include <stdio.h>
#include <conio.h>

#define FUJI_DEV 0x0F
#define MAX_URL (256)
#define ACK		 0x80

typedef struct
{
	unsigned char century, // Century
		year,			   // Year
		month,			   // Month
		day,			   // Day
		hour,			   // Hour
		minute,			   // Minute
		second;			   // Second
} FUJI_TIME;


typedef struct
{
	unsigned char cmd;
	unsigned short mode;
	unsigned char trans;
	unsigned char url[MAX_URL];
} FUJI_CMD;


int io_time(FUJI_TIME *time)
{
	FUJI_CMD oc;
	unsigned char response[1024];

	unsigned char r = 0;

	oc.cmd = 0xD2; // Get Time

	// request time
	if (eos_write_character_device(FUJI_DEV, (unsigned char *)oc, (unsigned short)sizeof(oc)) != ACK)
	{
		return 1; // could not open
	}

	r = eos_read_character_device(FUJI_DEV, response, (unsigned short)sizeof(response));
	if (r != ACK)
	{
		return 3; // did not get result
	}

	memcpy(time, response, sizeof(time));
	return 0;
}

void main(void)
{
    FUJI_TIME fuji_time;

	clrscr();

    io_time(&fuji_time);

	printf("\n\nDirectly from Fujinet:\n    Date: %02u%02u-%02u-%02u\n    Time: %02u:%02u:%02u\n",
		   fuji_time.century,	// Century
		   fuji_time.year,	    // Year
		   fuji_time.month,	    // Month
		   fuji_time.day,	    // Day
		   fuji_time.hour,	    // Hour
		   fuji_time.minute,    // Minute
		   fuji_time.second);   // Second


	while (1) {}
};