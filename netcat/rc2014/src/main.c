/**
 * Network Function Testbed
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "console.h"
#include "fujinet.h"
#include "fujinet_logical.h"
#include "fujinet_network.h"

unsigned char buf[1024];
unsigned char txbuf[64];
unsigned char t=0;

struct network_status status;
static bool done = false;

char url[256];

FUJINET_LOGICAL_DEVICE_TYPE src_logical_type = FUJINET_LOGICAL_DEVICE_TYPE_UNKNOWN;
uint8_t src_logical_unit = 0;
const char* src_logical_url = NULL;


void in(void)
{
    if (fujinet_poll_proceed()) {
        FUJINET_RC rc = fujinet_network_status(src_logical_unit, &status);

        if (rc == FUJINET_RC_OK) {
            uint16_t l = (status.bytes_waiting_high << 8) + status.bytes_waiting_low;
            if (l == 0 && status.protocol_status == 0)
                done = true;

            //printf("reading %u bytes\n", l);
            if (l > 0) {
                rc = fujinet_network_read(src_logical_unit, buf, l);
                //printf("read %u bytes (%u)\n", l, rc);
                if (rc == FUJINET_RC_OK) {
                    for (uint16_t i = 0; i < l; i++) {
                        putchar(buf[i]);
                    }
                }
            }
        }
    }
}

void out(void)
{
    char k;

    k = console_rx_avail();
    if (k > 0) {
        txbuf[0] = console_rx();
        if (txbuf[0] == 26) {
            done = true;
        } else {
            fujinet_network_write(src_logical_unit, txbuf, 1); // send one char.
        }
    }
}

int main(int argc, char **argv)
{
//    clrscr();
    fujinet_init();

    printf("Welcome to netcat, an example #FujiNet Network Program\n"
           "This is a very simple 80 column terminal program.\n\n"
           "Available protocols: TCP UDP TELNET HTTP/S TNFS FTP\n\n"
           "URL's are of the form:\n\n"
           "PROTO://host.name:port/path?key=val&key=val\n\n"
           "Examples of URL's:\n"
           "------------------\n"
           "TELNET://RC2014.DDNS.NET:2014/\n"
           "TELNET://BBS.FOZZTEXX.NET:23/\n"
           "HTTPS://www.gnu.org/licenses/gpl-3.0.txt\n\n");

    if (argc == 1) {
        printf("Usage: netcat <URL>\n");
        exit(1);
    }

    strcpy(url, argv[1]);

    printf("OPEN URL: %s\n", url);
    printf("OPEN RETURN: %02X\n", fujinet_network_open(src_logical_unit, url, 0));

    while(!done) {
        in();
        out();
    }

    fujinet_network_close(src_logical_unit);

    return 0;
}
