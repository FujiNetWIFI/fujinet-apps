/**
 * @brief   Netcat, a dumb terminal.
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Main program
 */

#include <cmoc.h>
#include <coco.h>
#include "net.h"

NetworkStatus ns;

byte rxBuf[2048];
byte showCursor = 1;

int open_connection(void)
{
    char *s;

    char url[256], login[256], password[256];

    width(40);
    cls(5);

    printf("WELCOME TO NETCAT\n");
    printf("ENTER URL, e.g.\n");
    printf("N:TELNET://BBS.FOZZTEXX.COM/\n");
    printf("\n\n");

    s = readline();
    strcpy(url,s);

    printf("\nENTER LOGIN, OR enter\nFOR NONE\n");
    s = readline();
    strcpy(login,s);
    net_login(0,login);
    
    printf("\nENTER PASSWORD, OR enter\nFOR NONE\n");
    s = readline();
    strcpy(password,s);
    net_password(0,password);
    
    return net_open(0,12,0,url);
}

void close_connection(void)
{
    net_close(0);
    printf("DISCONNECTED.\n");
}

byte in(void)
{
    net_status(0,&ns);

    if (!ns.bytesWaiting)
    {
        net_status(0,&ns);
        return net_error(0);
    }
    else if (ns.bytesWaiting > sizeof(rxBuf))
    {
        ns.bytesWaiting = sizeof(rxBuf);
    }
    
    net_read(0,rxBuf,ns.bytesWaiting);

    putchar('\x08'); // backspace cursor
    
    for (int i=0;i<ns.bytesWaiting;i++)
    {
        char c = rxBuf[i];

        if (c != 0x0A)
            putchar(c);
    }

    putchar(0xAF); // cursor.

    return net_error(0);
}

void out(void)
{
    byte k=inkey();

    if (k)
    {
        net_write(0,&k,1);
    }
}

byte nc(void)
{
    out();
    in();

    return net_error(0);
}

int main(void)
{
    setCaseFlag(0);
    
    if (!open_connection())
    {
        printf("OPEN ERR: %u\n",net_error(0));
        return 1;
    }

    net_status(0,&ns);

    while(nc() == 1);
    
    close_connection();

    setCaseFlag(1);
    
    return 0;
}
