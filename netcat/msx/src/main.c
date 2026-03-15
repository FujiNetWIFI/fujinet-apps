#include <conio.h>
#include <string.h>
#include <video/tms99x8.h>
#include <fujinet-network.h>

unsigned char cursor[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

char url[256]="N:TELNET://BBS.FOZZTEXX.COM/";
char url_input[256];

unsigned short bw=0;
unsigned char c=0;
unsigned char err=0;

unsigned char rxBuf[1024];

void out()
{
    while (kbhit())
    {
        char c=cgetc();
        network_write(url,&c,1);
    }
}

void in()
{
    network_status(url,&bw,&c,&err);

    while (bw)
    {
        unsigned short len = (bw > sizeof(rxBuf) ? sizeof(rxBuf) : bw);
        network_read(url,rxBuf,len);
        for (int i=0;i<len;i++)
        {
            cputc(rxBuf[i]);
            //vdp_put_sprite_16(0,wherex()*8,wherey()*8,0,6);
        }

        bw -= len;
    }
}

void main()
{
    network_init();
    vdp_set_mode(0);
    //vdp_set_sprite_mode(sprite_large);
    //vdp_set_sprite_16(0,cursor);
    clrscr();
    cprintf("WELCOME TO NETCAT!\n\n");
    cprintf("ENTER URL, OR <-| for\nN:TELNET://BBS FOZZTEXX.COM/\n");
    gets(url_input);

    if (url_input[0]!=0x00)
        strcpy(url,url_input);
    else
	strcpy(url,"N:TELNET://BBS.FOZZTEXX.COM/");

    cprintf("CONNECTING TO: %s\n\n",url);

    network_open(url,12,0);
    network_status(url,&bw,&c,&err);

    while (c)
    {
        out();
        in();
    }

    network_close(url);

    cprintf("\n\nDISCONNECTED.\n\n");

    while(1);
}
