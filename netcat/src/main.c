/**
 * A simple Netcat like program in CC65
 */

#include <atari.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> // for kbhit() and cgetc()
#include "conio.h" // our local one.
#include "nio.h"

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
bool echo=false;                // local echo?
extern void ih();               // defined in intr.s


/**
 * Get URL from user.
 */
void get_url()
{
  
  OS.lmargn=2; // Set left margin to 2
  OS.shflok=64; // turn on shift lock.
  print("Version: " __DATE__ " " __TIME__ "\x9b");
  print("NETCAT <DEVICESPEC>\x9b");
  print("(e.g.: N1:TCP://BBS.FOZZTEXX.NET/)\x9b");
  get_line(url,255);

  if (url[0] == '\x9b')
    strcpy(url, "N1:TCP://BBS.FOZZTEXX.NET/");
  
  print("\x9bTRANSLATION\x9B  0=NONE, 1=CR, 2=LF, *3=CR/LF?\x9b  *=Default\x9b");
  get_line(tmp,7);
  if (tmp[0] == '\x9b')
    trans = 3;
  else
    trans=atoi(tmp);
  
  print("\x9bLOCAL ECHO?--Y=YES, *N=NO? *=Default\x9b");
  get_line(tmp,7);
  echo=(tmp[0]=='Y' ? true : false);
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
  
  // Attempt open.
  print("\x9bOpening:\x9b");
  print(url);
  print("\x9b");

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

  // MAIN LOOP ///////////////////////////////////////////////////////////

  while (running==true)
  {
    // If key pressed, send it.
    if (kbhit())
    {
      char c=cgetc();
      err=nwrite(url,&c,1); // Send character.

      if (echo==true)
        printc(&c);

      if (err!=1)
        {
          print("WRITE ERROR: ");
          print_error(err);
          running=false;
          continue;
        }
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

/**
 * Main entrypoint
 */
void main(void)
{
  OS.soundr=0; // Turn off SIO beeping sound
  cursor(1);   // Keep cursor on
  
  while (running==true)
    {
      get_url();
      nc();
    }
  
  OS.soundr=3; // Restore SIO beeping sound
}
