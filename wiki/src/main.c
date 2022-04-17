/**
 * Wikipedia Article search and viewer for use with Atari FujiNet
 */

#include <atari.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "conio.h" // our local one.
#include "nio.h"

char baseurl[256];              // Base URL
char workingurl[256];           // Overwritten for each request
char search[115];               // Search Text
char article[16];               // Article id to get
unsigned short page = 1;        // Current page number
char newpage[3];                // Get new page number
char buf [sizeof(int)*8+1];
unsigned char err;              // error code of last operation.
bool running = true;            // When getting data
unsigned char trip=0;           // if trip=1, fujinet is asking us for attention.
bool old_enabled=false;         // were interrupts enabled for old vector
void* old_vprced;               // old PROCEED vector, restored on exit.
unsigned short bw=0;            // # of bytes waiting.
unsigned char rx_buf[8192];     // RX buffer.
unsigned char tx_buf[64];       // TX buffer.

extern void ih();               // defined in intr.s

/**
 * Replace spaces in a string
 */
void space_replace( char * old )
{
    int length = strlen(old);
    int i = 0; 
    for (; i < length; i++) {
        if( old[i] == ' ' )
            old[i] = '+';
        }
}

/**
 * Get and display search results or article contents
 */
void get_contents()
{
    err = nopen(workingurl,0);
    if (err != SUCCESS)
    {
        print("OPEN ERROR");
        return;
    }

    running=true;
    // Open successful, set up interrupt
    old_vprced  = OS.vprced;     // save the old interrupt vector 
    old_enabled = PIA.pactl & 1; // keep track of old interrupt state
    PIA.pactl  &= (~1);          // Turn off interrupts before changing vector
    OS.vprced   = ih;            // Set PROCEED interrupt vector to our interrupt handler.
    PIA.pactl  |= 1;             // Indicate to PIA we are ready for PROCEED interrupt.

    while(running==true){
        if (trip==0) // is nothing waiting for us?
            continue;

        // Something waiting for us, get status and bytes waiting.
        err=nstatus(workingurl);

        if (err==136)
        {
            //print("DISCONNECTED.");
            running=false;
            continue;
        }
        else if (err!=1)
        {
            print("STATUS ERROR");
            running=false;
            continue;
        }

        // Get # of bytes waiting, no more than size of rx_buf
        bw=OS.dvstat[1]*256+OS.dvstat[0];

        if (bw>sizeof(rx_buf))
            bw=sizeof(rx_buf);

        if (bw>0)
        {
            err=nread(workingurl,rx_buf,bw);

            if (err!=1)
            {
                print("READ ERROR");
                running=false;
                continue;
            }

            // Print the buffer to screen.
            printl(rx_buf,bw);
            
            trip=0;
            PIA.pactl |= 1; // Flag interrupt as serviced, ready for next one.
        } // if bw > 0
    }
    nclose(workingurl);
}

/**
 * Main Loop
 */
void main()
{
    // Build the base url
    strcpy(baseurl, "N:HTTPS://fujinet.online/8bitwiki/wiki.php?t=a&l=10&ps=39x22");
    strcpy(workingurl, baseurl);

    // Get search term
    print("\x9b");
    print(" == FujiNet Wikipedia Article Viewer ==\x9b");
    print("\x9b");
    print("Enter Search Term:\x9b");
    get_line(search,115);
    if (search[0] == '\x9b')
        strcat(workingurl, "&s=Taco");
    else
    {
        space_replace(search);
        strcat(workingurl, "&s=");
        strcat(workingurl, search);
    }

    // Do the search
    get_contents();

    // Get article number
    print("Enter ID:");
    get_line(article,16);
    strcpy(workingurl, baseurl);
    strcat(workingurl, "&a=");
    strcat(workingurl, article);

    // Show page 1
    get_contents();

    while(1)
    {
        // Get page contents
        strcpy(workingurl, baseurl);
        strcat(workingurl, "&p=");
        print("Enter Page Number to View:");
        get_line(newpage, sizeof(newpage));
        if (newpage[0] == '\x9b')
            page++;
        if (newpage == 0)
            page++;
        else
        {
            page = atoi(newpage);
        }
        itoa(page, buf, 10);
        strcat(workingurl, buf);
        strcat(workingurl, "&a=");
        strcat(workingurl, article);

        // Show the content
        get_contents();
    }
}