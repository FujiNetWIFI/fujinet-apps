/**
 * @brief   Lobby program for #FujiNet
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>
#include "appkey.h"
#include "nio.h"

#define CREATOR_ID 0x0001 /* FUJINET  */
#define APP_ID     0x01   /* LOBBY    */
#define KEY_ID     0x00   /* USERNAME */

#define SERVER     "N:TCP://IRATA.ONLINE:1512/"

unsigned char username[64];

/**
 * @brief The initial banner 
 */
void banner(void)
{
  cursor(1);
  revers(1);
  printf(" #FUJINET GAME LOBBY \n");
  revers(0);
}

void connect(void)
{
  unsigned char err;
  
  if ((err = nopen(SERVER,OREADWRITE)) != SUCCESS)
    {
      nstatus(SERVER);

      printf("Could not connect.\nError: %u\n",OS.dvstat[3]);
      exit(1);
    }
}

/**
 * @brief get User appkey, if exists
 * @return true if key fetched and set, otherwise false.
 */
bool get_user_appkey(void)
{
  if (sio_openkey(APPKEY_READ,CREATOR_ID,APP_ID,KEY_ID) != SIO_ERR_SUCCESS)
    return false;

  if (sio_readkey(username) != SIO_ERR_SUCCESS)
    return false;

  return true;
}

/**
 * @brief Get username and set key
 */
void get_user(void)
{
  memset(username,0,sizeof(username));
  
  while (strlen((const char *)username)<1)
    {
      printf("Enter a user name, and press RETURN.\n");
      gets((char *)username);
    }

  sio_openkey(APPKEY_WRITE,CREATOR_ID,APP_ID,KEY_ID);
  sio_writekey(username);  
}

/**
 * @brief Set user name, either from appkey or via input
 */
void register_user(void)
{  
  // Check for existing user
  if (get_user_appkey())
    {
      printf("User name is: %s\nPress C to change,\n any other key to continue.\n",username);

      switch(cgetc())
	{
	case 'c':
	case 'C':
	  get_user();
	  break;
	}
    }
  else
    get_user();
}

void main(void)
{
  clrscr();
  banner();
  register_user();
  connect();
}
