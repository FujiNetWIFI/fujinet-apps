/**
 * temporary fetch for pmd85, because no fujinet-lib for it yet.
 */

#include <string.h>
#include <stdint.h>

#include "net.h"

#define SUCCESS 1


/**
 * @brief our URL
 */
static char devicespec[]="N:HTTPS://fujinet.online/2024pres/";

/**
 * Our fetch buffer
 */
char s[128];

/**
 * network status
 */
static NetworkStatus ns;

#ifdef FETCH_MOCK
void fetch(void)
{
    strcpy(s,"Harris|38|Trump|42|DEMS|28|REPS|38|DEMH|113|REPH|134|Nov 3, 2024|9:14pm UTC");
}
# else

void fetch(void)
{
    uint16_t bw = 0;

    /* Open */
    if (net_open(0, 4, 0, devicespec) != SUCCESS) 
        return;

    /* GET, wait for data */
    while (!bw)
    {
        if (net_status(0, &ns) != SUCCESS)
            return;
        bw = ns.bytesWaiting;
    }
    if (bw > sizeof(s)) bw = sizeof(s);

    /* Read */
    net_read(0, (unsigned char *)s, bw);
    s[sizeof(s)-1] = '\0';

    /* Close */
    net_close(0);
}

# endif