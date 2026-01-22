/**
 * @brief   Netcat, a dumb terminal.
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Main program
 */

#include <cmoc.h>
#include <coco.h>

#include <fujinet-fuji.h>
#include <fujinet-network.h>
#include "net.h"
#include "cocotext.h"

uint16_t bytesWaiting;
unsigned char conn;
unsigned char error;

byte rxBuf[2048];
char url[256];

void presskey(void)
{
    printf("PRESS ANY KEY TO CLOSE...\n");
    waitkey(0);
}

int open_connection(void)
{
    char s[256];    

    char url[256], login[256], password[256];

    clear_screen(5);

    printf("WELCOME TO NETCAT\n");
    printf("ENTER URL, e.g.\n");
    printf("N:TELNET://BBS.FOZZTEXX.COM/\n");
    printf("\n\n");
    get_line(url,255);

    printf("\nENTER LOGIN, OR enter\nFOR NONE\n");

    get_line(login,255);
    net_login(url,login);
    
    printf("\nENTER PASSWORD, OR enter\nFOR NONE\n");
    get_line(password,255);
    net_password(url,password);
    
    return network_open(url, OPEN_MODE_RW, OPEN_TRANS_NONE);
}

void close_connection(void)
{
    network_close(url);
    printf("DISCONNECTED.\n");
}

byte in(void)
{
    network_status(url, &bytesWaiting, &conn, &error);

    if (!bytesWaiting)
    {
        network_status(url, &bytesWaiting, &conn, &error);
        return fn_error(error);
    }
    else if (bytesWaiting > sizeof(rxBuf))
    {
        bytesWaiting = sizeof(rxBuf);
    }
    
    network_read(url, rxBuf, bytesWaiting);

    putchar('\x08'); // backspace cursor
    
    for (int i=0;i<bytesWaiting;i++)
    {
        char c = rxBuf[i];

        if (c != 0x0A)
            putchar(c);
    }

    putchar(0xAF); // cursor.

    return fn_error(error);
}

void out(void)
{
    cursor(false);
    byte k=cgetc();

    if (k)
    {
        network_write(url, &k, 1);
    }
}

byte nc(void)
{
    out();
    in();

    return conn;
}

int main(void)
{
    initCoCoSupport();
    textMode = getTextMode();
    clear_screen(5);

    if (isCoCo3)
    {
        printf("(H)IRES 42, (4)0 OR (8)0 COLUMNS?");
        char mode = 0;
        bool modeselect_done = false;

		while (!modeselect_done)
		{
            mode = cgetc();

			switch (mode)
			{
			case '4':
				set_text_width(40);
                modeselect_done = true;
				break;
			case '8':
				set_text_width(80);
                modeselect_done = true;
                break;
			case 'h':
			case 'H':
				set_text_width(42);
                modeselect_done = true;
				break;
			}
		}
	}
    else
    {
        set_text_width(42);
    }
    
    cursor(false);

    if (open_connection())
    {
        printf("OPEN ERR: %u\n",fn_error(0));
        presskey();
        hirestxt_close();
        return 1;
    }

    network_status(url, &bytesWaiting, &conn, &error);

    while(nc() == 1);
    
    close_connection();
    
    presskey();

    hirestxt_close();
    return 0;
}
