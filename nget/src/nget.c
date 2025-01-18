/**
 * @brief NGET - Copy from network to local
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <fujinet-network.h>
#include <stdio.h>
#include "local_file_init.h"

#define NET_EOF 136

unsigned char buf[1024];

/**
 * @brief Do network transfer to local filesystem
 * @param source The source N: URL
 * @param dest the destination local file
 * @return error code
 */
int nget(char *source, char *dest)
{
    unsigned char err=0xFF;
    FILE *fp;
    unsigned char c=0, nerr=0;
    unsigned int bw=0;
    unsigned long total=0;
    
    err = network_init();
    if (err != FN_ERR_OK)
    {
        printf("Could not find Fujinet. Aborting.\n");
        goto nget_close;
    }

    local_file_init();
    err = network_open(source, OPEN_MODE_READ, OPEN_TRANS_NONE);
    if (err != FN_ERR_OK)
    {
        printf("Could not open %s, error #%u. Aborting.\n",source,err);
        goto nget_close;
    }


    fp = fopen(dest,"w");
    if (!fp)
    {
        perror("Could not open destination file. Aborting");
        goto nget_close;
    }

    err = network_status(source, &bw, &c, &nerr);
    if (err != FN_ERR_OK)
    {
        printf("Could not get initial network status. Aborting with error %u.\n",err);
        goto nget_close;
    }

    while (nerr != NET_EOF)
    {
        unsigned short wlen=0;
        
        if (bw)
        {
            if (bw > sizeof(buf))
                bw = sizeof(buf);
           
            network_read(source, buf, bw);

            wlen = fwrite(buf, sizeof(unsigned char), bw, fp);
            if (wlen != bw)
            {
                perror("Could not complete write to local file");
                goto nget_close;
            }

            printf(".");
        }
        
        err = network_status(source, &bw, &c, &nerr);
        
        if (err != FN_ERR_OK)
        {
            printf("Could not fetch subsequent network status. Aborting with error %u\n",err);
            goto nget_close;
        }        
    }

    printf("Done.\n");
    
nget_close:
    network_close(source);

    if (fp)
        fclose(fp);

    printf("PRESS ENTER TO CONTINUE.\n");
    gets(buf);
}

