/**
 * @brief chunked transfer test program
 */

#include <atari.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "conio.h"
#include "nio.h"
#include "intr.h"

#define TRANS_CRLF 2
#define SUCCESS    1

bool running=true;
const char *url="N:HTTPS://fujinet.online/chunk.php\x9b";
unsigned char buf[16384]; // RX buffer
unsigned char err;        // error for nio commands

unsigned char trip=0;     // if trip=1, fujinet is asking us for attention.
bool old_enabled=false;   // were interrupts enabled for old vector
void* old_vprced;         // old PROCEED vector, restored on exit.
unsigned short bw=0;      // # of bytes waiting.

void die(void)
{
  while(1);
}

void print_status(void)
{
  char cbw[6], ccon[3], cerr[4];

  bzero(cbw,sizeof(cbw));
  bzero(ccon,sizeof(ccon));
  bzero(cerr,sizeof(cerr));
  
  bw=OS.dvstat[1]*256+OS.dvstat[0];
  itoa(bw,cbw,10);
  itoa(OS.dvstat[2],ccon,10);
  itoa(OS.dvstat[3],cerr,10);

  print("DVSTAT: ");
  print(cbw);
  print(",");
  print(ccon);
  print(",");
  print(cerr);
  print("\x9b\x9b");
}

void main(void)
{
  print("CHUNKED TRANSFER TEST\x9b\x9b");

  print("OPENING:\x9b");
  print(url);
  print("\x9b");

  err = nopen(url,TRANS_CRLF);

  if (err != SUCCESS)
    {
      print_error(err);
      die();
    }

  // Open successful, set up interrupt
  old_vprced  = OS.vprced;     // save the old interrupt vector 
  old_enabled = PIA.pactl & 1; // keep track of old interrupt state
  PIA.pactl  &= (~1);          // Turn off interrupts before changing vector
  OS.vprced   = ih;            // Set PROCEED interrupt vector to our interrupt handler.
  PIA.pactl  |= 1;             // Indicate to PIA we are ready for PROCEED interrupt.

  while (running)
    {
      // If nothing waiting for us, wait.
      if (!trip)
	continue;

      // Get # of bytes waiting via status
      err=nstatus(url);

      if (err != SUCCESS)
	{
	  print("STATUS ERROR\x9b");
	  print_error(err);
	  print_status();
	  nclose(url);
	  die();
	}

      bw = OS.dvstat[1]*256+OS.dvstat[0];

      if (bw>sizeof(buf))
	bw=sizeof(buf);

      if (bw>0)
	{
	  err = nread(url,buf,bw);

	  if (err != SUCCESS)
	    {
	      print("READ ERROR\x9b");
	      print_error(err);
	      print_status();
	      nclose(url);
	      die();
	    }
	  else
	    {
	      // Print to screen
	      printl((const char *)buf,bw);
	    }
	  
	}
      trip=0;
      PIA.pactl |= 1; // Interrupt serviced.
    }
    
}
