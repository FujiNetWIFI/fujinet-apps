/**
 * @brief Fetch data from Mozzwald's server
 */

#ifdef _COCO_BASIC_ 
#include <cmoc.h>
#include <coco.h>
#else
#include <string.h>
#endif /* __CMOC_VERSION */

#include <fujinet-network.h>

/**
 * @brief our URL
 */
char devicespec[]="N:HTTPS://fujinet.online/2024pres/";

/**
 * Our fetch buffer
 */
char s[128];

void fetch(void)
{
    uint16_t bw = 0;
    uint8_t c = 0, err = 0;

    printf("INIT\n");

    network_init();

    printf("OPEN\n");
    
    if (network_open(devicespec, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE)
        != FN_ERR_OK)
    {
        return;
    }

    printf("STATUS\n");
    
    while (!bw)
        network_status(devicespec, &bw, &c, &err);

    bw = 77;

    printf("BW: %u C: %u ERR: %u",bw,c,err);

    printf("READ\n");
    network_read(devicespec, (unsigned char *)s, bw);

    printf("S: %s\n",s);

    printf("CLOSE\n");
    network_close(devicespec);

    /* strcpy(s,"Harris|38|Trump|42|DEMS|28|REPS|38|DEMH|113|REPH|134|Nov 3, 2024|9:14pm UTC"); */
}
