/**
 * temporary fetch for adam, because no fujinet-lib for it yet.
 */

#include <conio.h>
#include <smartkeys.h>
#include <eos.h>
#include <string.h>
#include <stdlib.h>

#define NET_DEV 0x09
#define ACK 0x80
#define NACK 0x8C

unsigned char response[1024];

struct _oc
{
    unsigned char cmd;
    unsigned char mode;
    unsigned char trans;
    unsigned char url[256];
} oc = {'O',4,0,"N:HTTPS://fujinet.online/2024pres/"};

char s[128];

struct _status
{
    unsigned short bw;
    unsigned char c;
    unsigned char err;
}
status;

unsigned char network_open(void)
{
    return eos_write_character_device(NET_DEV,(unsigned char *)oc,sizeof(struct _oc));
}

unsigned char network_close(void)
{
    return eos_write_character_device(NET_DEV,"C",1);
}

unsigned char network_status(void)
{
    unsigned char r=0;

    r=eos_write_character_device(NET_DEV,"S",1);

    if (r != ACK)
        return r;

    r = eos_read_character_device(NET_DEV,response,sizeof(response));

    memcpy(status,response,sizeof(struct _status));

    return r;
}

unsigned short network_read(char *buf, unsigned short len)
{
    DCB *dcb = eos_find_dcb(NET_DEV);
    unsigned char r=eos_read_character_device(NET_DEV,response,sizeof(response));

    if (r == ACK)
    {
        memcpy(buf,response,dcb->len);
        return dcb->len;
    }
    else
        return 0;
}

void fetch(void)
{
    if (network_open() != ACK)
        return;

    network_status();

    network_read(s,status.bw);

    network_close();    
}
