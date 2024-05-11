/**
 * @brief   HTTP Leak Analysis
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Repeated calls of HTTP read and parse.
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdint.h>
#include <fujinet-network.h>

char *http_url = "N:HTTP://api.open-notify.org/astros.json";
char *https_url = "N:HTTPS://oldbytes.space/api/v1/timelines/public?limit=1";

uint8_t buf[16384];
uint16_t bw;
uint8_t c;
uint8_t err;

void http_no_parse(void)
{
    unsigned char retries=5;

    printf("HTTP NO PARSE, 5 ATTEMPTS.\n\n");

    while(retries--)
    {
        network_open(http_url,12,0);
        network_status(http_url,&bw,&c,&err);
        network_read(http_url,buf,bw);
        network_close(http_url);
        printf("ATTEMPT #%u - BW: %u BYTES. PRESS ENTER. ",retries,bw);
        getchar();
    }
}

void http_parse(void)
{
    unsigned char retries=5;

    printf("HTTP JSON PARSE, 5 ATTEMPTS.\n\n");
    
    while (retries--)
    {
        network_open(http_url,12,0);
        network_status(http_url,&bw,&c,&err);

        network_http_set_channel_mode(http_url,1);
        network_json_parse(http_url);
        
        network_close(http_url);
        printf("ATTEMPT #%u - BW: %u BYTES. PRESS ENTER. ",retries,bw);
    }
}

void https_no_parse(void)
{
    unsigned char retries=5;

    printf("HTTPS NO PARSE, 5 ATTEMPTS.\n\n");

    while(retries--)
    {
        network_open(https_url,12,0);
        network_status(https_url,&bw,&c,&err);
        network_read(https_url,buf,bw);
        network_close(https_url);
        printf("ATTEMPT #%u - BW: %u BYTES. PRESS ENTER. ",retries,bw);
        getchar();
    }
}

void https_parse(void)
{
    unsigned char retries=5;

    printf("HTTPS JSON PARSE, 5 ATTEMPTS.\n\n");
    
    while (retries--)
    {
        network_open(https_url,12,0);
        network_status(https_url,&bw,&c,&err);

        network_http_set_channel_mode(https_url,1);
        network_json_parse(https_url);
        
        network_close(https_url);
        printf("ATTEMPT #%u - BW: %u BYTES. PRESS ENTER. ",retries,bw);
    }
}

void main(void)
{
    unsigned char i=0;

    network_init();
    
    for (i=0;i<5;i++)
        http_no_parse();

    for (i=0;i<5;i++)
        http_parse();

    for (i=0;i<5;i++)
        https_no_parse();

    for (i=0;i<5;i++)
        https_parse();
}
