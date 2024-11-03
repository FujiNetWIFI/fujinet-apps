/**
 * @brief Fetch data from Mozzwald's server
 */

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
    
    if (network_open(devicespec, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE)
        != FN_ERR_OK)
    {
        return;
    }

    while (!bw)
        network_status(devicespec, &bw, &c, &err);

    network_read(devicespec, (unsigned char *)s, bw);
    
    network_close(devicespec);
}
