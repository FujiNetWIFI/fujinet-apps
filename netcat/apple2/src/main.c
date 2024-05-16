/**
 * @brief   NetCat - a simple Terminal Emulator
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Main Program
 */

#include "read_line.h"
#include <apple2.h>
#include <fujinet-network.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t rxbuf[8192];

char devicespec[256];
uint16_t bw;
uint8_t c;
uint8_t err;

bool connect(void)
{
    network_init();
    videomode(VIDEOMODE_80COL);
    clrscr();
    
    printf("WELCOME TO #FUJINET NETCAT V2.0\n");
    printf("-------------------------------\n\n");

    printf("ENTER A URL IN THE FORM:\n\n");
    printf("N:PROTO://HOST.NAME:PORT/[PATH...]\n\n");

    printf("EXAMPLES OF N: URLs:\n");
    printf("--------------------\n");
    printf("N:TELNET://BBS.FOZZTEXX.COM/\n");
    printf("N:TCP://IRATA.ONLINE:6502/\n");
    printf("N:SSH://MYLINUXHOST/\n");
    printf("N:HTTPS://www.gnu.org/licenses/gpl-3.0.txt\n\n");

    cursor(1);
    read_line(devicespec,sizeof(devicespec),false);

    printf("\n\n");

    network_open(devicespec, OPEN_MODE_RW, OPEN_TRANS_NONE);
    return true;
    /* return network_open(devicespec, OPEN_MODE_RW, OPEN_TRANS_NONE); */
}

void esc_to_quit_or_restart(void)
{
    cprintf(" ESC TO QUIT, OR ANY KEY TO RESTART.");
    
    if (cgetc() == CH_ESC)
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

        putchar(0x08);

        for (i=0;i<bw;i++)
            if (rxbuf[i]!=0x0D)
                putchar(rxbuf[i]);

        revers(1);
        putchar(0x20);
        revers(0);
    }
}

void out(void)
{
    if (kbhit())
    {
        char c = cgetc();
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
        printf("COULD NOT CONNECT. ");
        esc_to_quit_or_restart();
    }

    revers(1);
    putchar(0x20);
    revers(0);
    
    while (is_connected())
        netcat();

    printf("DISCONNECTED. ");
    esc_to_quit_or_restart();
    goto restart;
}
