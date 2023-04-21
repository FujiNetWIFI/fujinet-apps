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
void* old_vprced;               // old PROCEED vector, restored on exit.
bool old_enabled=false;         // were interrupts enabled for old vector
bool running=false;
unsigned short bw=0;            // # of bytes waiting.
unsigned char rx_buf[1024];     // RX buffer.
unsigned char tx_buf[128];       // TX buffer.
unsigned char txbuflen;         // TX buffer length
unsigned char i;
unsigned char trip=0;           // if trip=1, fujinet is asking us for attention.
bool echo = true;

extern void ih();               // defined in intr.s

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

void term(void)
{
  unsigned char err;

  running = true;
  
  while (running==true)
  {
    if (kbhit())
      {
	memset(tx_buf,0,sizeof(tx_buf));
	printf(">> ");
	cursor(1);
	gets((char *)tx_buf);
	if (strlen((char *)tx_buf))
	  {
	    nwrite(SERVER,tx_buf,strlen((char *)tx_buf));
	    nwrite(SERVER,"\x9b",1);
	  }
      }
    
    if (trip==0) // is nothing waiting for us?
      continue;

    // Something waiting for us, get status and bytes waiting.
    err=nstatus(SERVER);

    if (err==136)
    {
      printf("DISCONNECTED.\x9b");
      running=false;
      continue;
    }
    else if (err!=1)
    {
      printf("STATUS ERROR: %u\n",err);
      running=false;
      continue;
    }

    // Get # of bytes waiting, no more than size of rx_buf
    bw=OS.dvstat[1]*256+OS.dvstat[0];

    if (bw>sizeof(rx_buf))
      bw=sizeof(rx_buf);
    
    if (bw>0)
    {
      err=nread(SERVER,rx_buf,bw);

      if (err!=1)
        {
          printf("READ ERROR: %u\n",err);
          running=false;
          continue;
        }

      // Print the buffer to screen.
      for (i=0;i<bw;i++)
	putchar(rx_buf[i]);
      
      trip=0;
      PIA.pactl |= 1; // Flag interrupt as serviced, ready for next one.
    } // if bw > 0
  } // while running
}

void connect(void)
{
  unsigned char err;
  char login[80];
  
  if ((err = nopen(SERVER,2)) != SUCCESS)
    {
      nstatus(SERVER);

      printf("Could not connect.\nError: %u\n",OS.dvstat[3]);
      exit(1);
    }

  // Open successful, set up interrupt
  old_vprced  = OS.vprced;     // save the old interrupt vector 
  old_enabled = PIA.pactl & 1; // keep track of old interrupt state
  PIA.pactl  &= (~1);          // Turn off interrupts before changing vector
  OS.vprced   = ih;            // Set PROCEED interrupt vector to our interrupt handler.
  PIA.pactl  |= 1;             // Indicate to PIA we are ready for PROCEED interrupt.
  
  // Send login
  sprintf(login,"/login %s\n\n",username);
  nwrite(SERVER,(unsigned char *)login,strlen(login));
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
  term();
}
