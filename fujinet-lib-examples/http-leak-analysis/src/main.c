/**
 * @brief   HTTP Leak Analysis
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Repeated calls of HTTP read and parse.
 */

#include <conio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <fujinet-network.h>

char *http_url = "n:http://api.open-notify.org/astros.json";
char *https_url = "n:https://oldbytes.space/api/v1/timelines/public?limit=1";
char tcp_url[80];   // = "N:TCP://TMA-2:6502/";
char tnfs_url[80];  // = "N:TNFS://TMA-2/TEST.DAT";

uint8_t buf[16384];
uint16_t bw;
uint8_t c;
uint8_t err;

void set_urls()
{
    sprintf(tcp_url, "n:tcp://%s:%s/", TCP_SERVER_ADDRESS, TCP_SERVER_PORT);    
    sprintf(tnfs_url, "n:tnfs://%s", TNFS_FILE_PATH);    
}

void http_no_parse(void)
{
    unsigned char retries=5;

    printf("\nHTTP NO PARSE, 5 ATTEMPTS.\n");

    while(retries--)
    {
        network_open(http_url,12,0);
        network_status(http_url,&bw,&c,&err);
        network_read(http_url,buf,bw);
        network_close(http_url);
        printf("#%u - BW: %u BYTES. PRESS ENTER.", retries, bw);
        getchar();
    }
}

void http_parse(void)
{
    unsigned char retries=5;

    printf("\nHTTP JSON PARSE, 5 ATTEMPTS.\n");
    
    while (retries--)
    {
        network_open(http_url,12,0);
        network_status(http_url,&bw,&c,&err);

        network_http_set_channel_mode(http_url,1);
        network_json_parse(http_url);
        
        network_close(http_url);
        printf("#%u - BW: %u BYTES. PRESS ENTER.", retries, bw);
        getchar();
    }
}

void https_no_parse(void)
{
    unsigned char retries=5;

    printf("\nHTTPS NO PARSE, 5 ATTEMPTS.\n");

    while(retries--)
    {
        network_open(https_url,12,0);
        network_status(https_url,&bw,&c,&err);
        network_read(https_url,buf,bw);
        network_close(https_url);
        printf("#%u - BW: %u BYTES. PRESS ENTER.", retries, bw);
        getchar();
    }
}

void https_parse(void)
{
    unsigned char retries=5;

    printf("\nHTTPS JSON PARSE, 5 ATTEMPTS.\n");
    
    while (retries--)
    {
        network_open(https_url,12,0);
        network_status(https_url,&bw,&c,&err);

        network_http_set_channel_mode(https_url,1);
        network_json_parse(https_url);
        
        network_close(https_url);
        printf("#%u - BW: %u BYTES. PRESS ENTER.", retries, bw);
        getchar();
    }
}

void tnfs_test(void)
{
    unsigned char retries = 5;
    
    printf("\nTNFS TEST, 5 ATTEMPTS.\n");

    while(retries--)
    {
        network_open(tnfs_url,12,0);
        network_status(tnfs_url,&bw,&c,&err);
        network_read(tnfs_url,buf,512);
        network_close(tnfs_url);

        printf("#%u - BW: 512 BYTES. PRESS ENTER.", retries);
        getchar();
    }
}

void http_open_close(void)
{
    unsigned char retries=5;

    printf("\nHTTP OPEN/CLOSE, 5 ATTEMPTS.\n");

    while(retries--)
    {
        network_open(http_url,12,0);
        network_close(http_url);
        printf("#%u - PRESS ENTER.", retries);
        getchar();
    }
}

void tcp_open_close(void)
{
    unsigned char retries=5;

    printf("\nTCP OPEN/CLOSE, 5 ATTEMPTS.\n");

    while(retries--)
    {
        network_open(tcp_url,12,0);
        network_close(tcp_url);
        printf("#%u - PRESS ENTER.", retries);
        getchar();
    }    
}

int main(void)
{
    set_urls();
    clrscr();
    printf("Leak Testing.\nPress a key to start.\n");
    cgetc();

    network_init();

    tcp_open_close();
    
    http_open_close();
    
    http_no_parse();

    http_parse();

    https_no_parse();

    https_parse();

    tnfs_test();

    printf("Testing Complete.\nPress a key to reboot.\n");
    cgetc();

    return 0;
}
