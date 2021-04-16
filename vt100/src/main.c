/**
 * ADM-3A Terminal Emulator derived from Netcat.
 */

#include <atari.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> // for kbhit() and cgetc()
#include "conio.h" // our local one.
#include "nio.h"
#include "term.h"

char url[256];                  // URL
bool running=true;              // Is program running?
unsigned char trans=3;          // Translation value (0,1,2,3)
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
 * Get URL from user.
 */
bool get_url(int argc, char* argv[])
{
  OS.shflok=64; // turn on shift lock.

  if (!_is_cmdline_dos())
    {
    interactive:
      print("VT100 TERMINAL--DEVICESPEC?\x9b");
      get_line(url,255);
                  
      print("\x9bUSERNAME?--RETURN IF NONE\x9b");
      get_line(login,128);

      print("\x9bPASSWORD?--RETURN IF NONE\x9b");
      get_line(password,128);

      nlogin(url,login,password);
    }
  else
    {
      strcpy(url,argv[1]);
      if (argc<2)
	goto interactive;
    }
  return true;
}

/**
 * Print error
 */
void print_error(unsigned char err)
{
  itoa(err,tmp,10);
  print(tmp);
  print("\x9b");
}

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
      return;
    }
  
  // Open successful, set up interrupt
  old_vprced  = OS.vprced;     // save the old interrupt vector 
  old_enabled = PIA.pactl & 1; // keep track of old interrupt state
  PIA.pactl  &= (~1);          // Turn off interrupts before changing vector
  OS.vprced   = ih;            // Set PROCEED interrupt vector to our interrupt handler.
  PIA.pactl  |= 1;             // Indicate to PIA we are ready for PROCEED interrupt.

  // Clear screen
  print("\x7D");
  
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

      // send buffer to terminal emulator.
      term(rx_buf,bw);
      
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

/**
 * Main entrypoint
 */
int main(int argc, char* argv[])
{
  OS.soundr=0; // Turn off SIO beeping sound
  cursor(1);   // Keep cursor on
  
  while (running==true)
    {
      if (get_url(argc, argv))
	nc();
      else
	running=false;
    }
  
  OS.soundr=3; // Restore SIO beeping sound
  return 0;
}
