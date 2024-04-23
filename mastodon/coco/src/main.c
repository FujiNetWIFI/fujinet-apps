/**
 * @brief View latest Mastodon Post from OLDBYTES.SPACE
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include "net.h"

#define false 0
#define true 1

#define SUCCESS 1

#define JSON 1

#define HTTP_GET 12
#define NO_TRANSLATION 0

#define MASTODON_SITE "N:HTTPS://OLDBYTES.SPACE/api/v1/timelines/public?limit=1"
#define QUERY_DISPLAY_NAME "/0/account/display_name"
#define QUERY_CREATED_AT "/0/created_at"
#define QUERY_CONTENT "/0/content"
#define MAX_SIZE 381

/* Output buffer */
byte buf[512];

/**
 * @brief the main buffer
 */
void clear_buf(void)
{
    memset(buf,0,sizeof(buf));
}

/**
 * @brief Do JSON query, and display the result
 * @param devid Device ID (0-255) 
 * @param qs The JQ query string
 */
void query_and_display_result(byte devid, const char *f, const char *qs)
{
    NetworkStatus ns;
    byte ellipsize = false;
    
    // First set the JSON query
    net_set_json_query(0,qs);

    // Get # of bytes waiting to be read, via status
    net_status(0,&ns);

    // Truncate to visible screen, if needed.
    if (ns.bytesWaiting > MAX_SIZE)
    {
        ellipsize = true;
        ns.bytesWaiting = MAX_SIZE;
    }

    // Clear the buffer before reading
    clear_buf();

    // Read the result into buffer
    net_read(0,buf,ns.bytesWaiting);

    // Display it.
    printf(f,buf);
    
    if (ellipsize)
    {
        printf("...");
    }
}

void mastodon(void)
{
    byte err=0xFF;

    // Banner
    printf("\x80\x80\x80\x80\x80\x80\x80latest\x80mastodon\x80post\x80\x80\x80\x80\x80");
    
    // Open the connection
    err = net_open(0,HTTP_GET,NO_TRANSLATION,MASTODON_SITE);

    if (err != SUCCESS)
    {
        printf("OPEN ERR: %u\n",err);
        return;
    }

    // Set channel mode to JSON
    net_set_channel_mode(0,JSON);

    // Perform the parse
    err = net_parse_json(0);

    if (err != SUCCESS)
    {
        printf("COULD NOT PARSE JSON\n");
        return;
    }

    // Display the results.
    query_and_display_result(0,"%32s",QUERY_DISPLAY_NAME);
    query_and_display_result(0,"%32s",QUERY_CREATED_AT);
    query_and_display_result(0,"%s",QUERY_CONTENT);

    // Close the connection
    net_close(0);    
}

int main(void)
{
    unsigned char *sp = (unsigned char *)0x5FF;
    unsigned char c=0x8F;
    
    while(1)
    {
        int t=200;
        cls(((unsigned char)rand())&0x03);
        mastodon();
        
        while (t) // while timer is counting down.
        {
            *sp = c;    // update cursor
            delay(6);   // wait a bit.
            c+=0x10;    // shift to next color
            if (c<0x8F) // If we're outside the SG4 charset, 
                c=0x8f; // readjust.
            t--;        // Count down.    
        }        
    }
    
    
    return 0;
}
