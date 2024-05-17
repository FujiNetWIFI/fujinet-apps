/**
 * @brief   NetCat - a simple Terminal Emulator
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Main Program
 */

#include "text.h"
#include "read_line.h"
#include <apple2.h>
#include <fujinet-network.h>
#include <conio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t rxbuf[8192];     // Receive buffer
char devicespec[256];    // Buffer for the device spec
char username[256];      // Username credentials
char password[256];      // Password credentials
uint16_t bw;             // # of bytes waiting (0-65535)
uint8_t c;               // Connected? (0=false, 1=true)
uint8_t err;             // Error code from most recent status

const char *banner =
//   ---------------------------------------|----------------------------------------
    "Welcome to FujiNet NetCat, Version 2.0   A Dumb Terminal Emulator\n\n"
    "Enter a URL in the form:\n\n"
    "N:PROTO://HOST.NAME:PORT/[PATH...]\n\n"
    "Some Protocols: TCP, UDP, HTTP/S, SSH,  FTP, TNFS, TELNET\n\n"
    "URL Examples:\n\n"
    "N:TELNET://BBS.FOZZTEXX.COM             N:TCP://IRATA.ONLINE:6502/\n"
    "N:SSH://MYLINUXHOST/\n"
    "\n\n"
    "ENTER URL\n\n"
    ">> ";

bool connect(void)
{
    check80column();

    if (is80column())
        init80();
    
    network_init();

    clrscr();
    
    outs(banner);
    
    read_line(devicespec,sizeof(devicespec),false);

    /* outs("\nUsername, or RETURN for none: "); */
    /* read_line(username,sizeof(username),false); */
    /* network_set_username(devicespec,username,len); */
    
    /* outs("\nPassword, or RETURN for none:"); */
    /* read_line(password,sizeof(password),true); */
    /* network_set_password(devicespec,password,len); */    
    
    clrscr();
    
    network_open(devicespec, OPEN_MODE_RW, OPEN_TRANS_NONE);
    return true;
}

void esc_to_quit_or_restart(void)
{
    outs(" ESC TO QUIT, OR ANY KEY TO RESTART.");
    
    if (inc() == CH_ESC)
        exit(1);
}

bool is_connected(void)
{
    network_status(devicespec,&bw,&c,&err);

    return c;
}

void in(void)
{
    uint16_t i=0;
    
    if (bw)
    {
        if (bw > sizeof(rxbuf))
            bw = sizeof(rxbuf);

        network_read(devicespec, rxbuf, bw);

        outc(CH_CURS_LEFT);

        for (i=0;i<bw;i++)
            if (rxbuf[i]!=0x0D)
                outc(rxbuf[i]);

        place_cursor();
    }
}

void out(void)
{
    if (kbhit())
    {
        char c = inc();
        network_write(devicespec,(uint8_t *)&c,1);
    }
}

void netcat(void)
{
    in();
    out();
}

int main(void)
{
 restart:
    while (!connect())
    {
        outs("COULD NOT CONNECT. ");
        esc_to_quit_or_restart();
    }

    place_cursor();
    
    while (is_connected())
        netcat();

    outs("DISCONNECTED. ");
    esc_to_quit_or_restart();
    goto restart;
}
