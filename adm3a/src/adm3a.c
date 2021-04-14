/**
 * N: ADM-3A Terminal Emulator
 *
 * Author: Thom Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Licensed under the GPL v3.0
 * See LICENSE for details.
 */

#include <conio.h>
#include <stdbool.h>
#include "adm3a.h"
#include "conio.h"

char url[256];                  // URL
bool running=true;              // Is program running?
unsigned char trans=0;          // Translation value (0,1,2,3)
char tmp[8];                    // temporary # to string
unsigned char err;              // error code of last operation.
unsigned char trip=0;           // if trip=1, fujinet is asking us for attention.
bool old_enabled=false;         // were interrupts enabled for old vector
void* old_vprced;               // old PROCEED vector, restored on exit.
unsigned short bw=0;            // # of bytes waiting.
unsigned char rx_buf[8192];     // RX buffer.
unsigned char tx_buf[64];       // TX buffer.
bool echo=false;                // local echo?
unsigned char txbuflen;         // TX buffer length
unsigned char i;
char login[256];                // username for login
char password[256];             // password for login

extern void ih();               // defined in intr.s

/**
 * NetCat
 */
void nc()
{
  OS.lmargn=0; // Set left margin to 0
  OS.shflok=0; // turn off shift-lock.
  
  err=nopen(url,trans);

  if (err != SUCCESS)
    {
      print("OPEN ERROR: ");
      print_error(err);
      running=false;
      return;
    }
  
  // Open successful, set up interrupt
  old_vprced  = OS.vprced;     // save the old interrupt vector 
  old_enabled = PIA.pactl & 1; // keep track of old interrupt state
  PIA.pactl  &= (~1);          // Turn off interrupts before changing vector
  OS.vprced   = ih;            // Set PROCEED interrupt vector to our interrupt handler.
  PIA.pactl  |= 1;             // Indicate to PIA we are ready for PROCEED interrupt.

  // MAIN LOOP ///////////////////////////////////////////////////////////

  while (running==true)
  {
    // If key pressed, send it.
    while (kbhit())
    {
      tx_buf[txbuflen++]=cgetc();
    }
    
    if (txbuflen>0)
      {
	if (echo==true)
	  for (i=0;i<txbuflen;i++)
	    printc(&tx_buf[i]);
	
	err=nwrite(url,tx_buf,txbuflen); // Send character.
	
	if (err!=1)
	  {
	    print("WRITE ERROR: ");
	    print_error(err);
	    running=false;
	    continue;
	  }
	txbuflen=0;
      }

    if (trip==0) // is nothing waiting for us?
      continue;

    // Something waiting for us, get status and bytes waiting.
    err=nstatus(url);

    if (err==136)
    {
      print("DISCONNECTED.\x9b");
      running=false;
      continue;
    }
    else if (err!=1)
    {
      print("STATUS ERROR: ");
      print_error(err);
      running=false;
      continue;
    }

    // Get # of bytes waiting, no more than size of rx_buf
    bw=OS.dvstat[1]*256+OS.dvstat[0];

    if (bw>sizeof(rx_buf))
      bw=sizeof(rx_buf);
    
    if (bw>0)
    {
      err=nread(url,rx_buf,bw);

      if (err!=1)
        {
          print("READ ERROR: ");
          print_error(err);
          running=false;
          continue;
        }

      // Print the buffer to screen.
      printl(rx_buf,bw);
      
      trip=0;
      PIA.pactl |= 1; // Flag interrupt as serviced, ready for next one.
    } // if bw > 0
  } // while running
  
  // END MAIN LOOP ///////////////////////////////////////////////////////
  
  // Restore old PROCEED interrupt.
  PIA.pactl &= ~1; // disable interrupts
  OS.vprced=old_vprced; 
  PIA.pactl |= old_enabled; 
    
}

int main(int argc, char* argv[])
{
  OS.soundr=0=OS.crsinh=0;

  while (running==true)
    {
      if (get_url(argc, argv))
	nc();
      else
	running=false;
    }
  
  OS.soundr=3;
  
  return 0;
}
