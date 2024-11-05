/**
 * @brief View latest Mastodon Post from OLDBYTES.SPACE
 * @author Thomas Cherryhomes, ported to Tesla PMD 85 by Jan Krupa
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <conio.h>
#include <ctype.h>
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
#define MAX_SIZE 720

/* Output buffer */
byte buf[1024];

/**
 * @brief Do JSON query, and display the result
 * @param devid Device ID (0-255) 
 * @param qs The JQ query string
 */
void query_json(byte devid, const char *qs)
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
    memset(buf,0,sizeof(buf));

    // Return if no data is available
    if (!ns.bytesWaiting)
        return;

    // Read the result into buffer
    net_read(0,buf,ns.bytesWaiting);

    // Replace some non-printable characters
    unsigned char *p = buf; char c;
    for(int i = 0; i < ns.bytesWaiting; i++)
    {
        c = *p;
        if (!c)
            break;
        if (c < 32 || c > 127)
            if (c!=10) *p = '.';
        p++;
    }
}


#define SCR_X0    4
#define SCR_Y0    3
static char tmp_str[41];

unsigned char strcut(char *d, char *s, unsigned char l)
{
	strncpy(d, s, l+1);
	if (d[l])
	{
		d[l] = '\0';
		return l;
	}
	return 0;
}

screen_print_multiline(unsigned char ypos, const char *longstr)
{
	char *s = longstr;
	unsigned char n;
	unsigned char y = ypos;
	do 
	{
		gotoxy(SCR_X0, y);
		n = strcut(tmp_str, s, 40);
		cprintf("%s", tmp_str);
		if (y < SCR_Y0 + 29) y++;
		s += n;
	} while (n);
}

void mastodon(void)
{
    byte err=0xFF;

    // Banner
    gotoxy(SCR_X0 + 8, SCR_Y0);
    printf("[ LATEST MASTODON POST ]");
    
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
    query_json(0, QUERY_DISPLAY_NAME);
    screen_print_multiline(SCR_Y0 + 2, buf);

    query_json(0, QUERY_CREATED_AT);
    screen_print_multiline(SCR_Y0 + 3, buf);

    query_json(0, QUERY_CONTENT);
    screen_print_multiline(SCR_Y0 + 5, buf);

    // Close the connection
    net_close(0);    
}

void idle(unsigned char t)
{
    char c = ' ';
    textcolor(RED);
    while(t--) {
        if (t < 5) c = '1' + t;
        gotoxy(SCR_X0+39, SCR_Y0+25); cprintf("\x1Bp%c", c);
        for(unsigned i=16500; i--;);
        gotoxy(SCR_X0+39, SCR_Y0+25); cputs("\x1Bq ");
        for(unsigned i=16500; i--;);
    }
    textcolor(GREEN);
}

int main(void)
{
    while(1)
    {
        clrscr();
        mastodon();
        idle(20);
    }
   
    return 0;
}
