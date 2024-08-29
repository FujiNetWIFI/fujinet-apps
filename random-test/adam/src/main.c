/**
 * #FujiNet Adam AppKey test
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <stdio.h>
#include <conio.h>
#include <msx.h>
#include <eos.h>

#define ADAMNET_FUJI_DEVICE_ID 0x0F

#define TEST_CREATOR_ID 0x0100
#define TEST_APP_ID 0x1F
#define TEST_KEY_ID 0xCC

#define ADAMNET_SEND_APPKEY_READ  0xDD
#define ADAMNET_SEND_APPKEY_WRITE 0xDE

#define MAX_APPKEY_LEN 64

#define MAX_DATA 2

unsigned char response[1024];

typedef struct
{
	unsigned char  cmd;
	unsigned short mode;
	unsigned char  trans;
	unsigned char  data[MAX_DATA];
} FUJI_CMD;



short random_read(void)
{
	FUJI_CMD oc;
	unsigned char response[1024];
  short num;


	unsigned char r = 0;

	oc.cmd = 0xD3; // random

	// request time
	if (eos_write_character_device(ADAMNET_FUJI_DEVICE_ID, (unsigned char *)oc, (unsigned short)sizeof(oc)) != 0x80)
	{
    printf("Write failure\n");
	} else
  {
    printf("Success write\n");
  }

  printf("%02x %02x", oc.data[0], oc.data[1]);
  num = 0;
  memcpy(&num, &oc.data, sizeof(num));

	return num;
}


void main(void)
{
  char wait[10];
  short num = 0;

  msx_set_mode(0);
  printf("\r\nRandom TEST " __DATE__ "  " __TIME__ "\n\n");
  num = random_read();
  printf("\n\n\tNumber: %d\n\n", num);

  gets(wait);

}
