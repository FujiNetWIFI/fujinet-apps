#ifdef __C64__

/*
  Network functionality
*/

#include <c64.h>
#include <cbm.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include "../platform-specific/appkey.h"
#include "../fujinet-fuji.h"
#include "../fujinet-network.h"

#define LFN 2     // The logical file number to use for I/O
#define DEV 16    // The network device #
#define SAN 2     // The secondary address (SA) to use on DEV.
#define CMD 15    // The secondary address for the Command channel

const char cmd_parse[]="jsonparse,2";
const char json_query[] = "jq,2,";

int getResponse(char *url, unsigned char *buffer, uint16_t max_len)
{
    static int16_t count;
    /* network_open(url, OPEN_MODE_HTTP_GET_H, OPEN_TRANS_NONE); */
    /* count = network_read(url, buffer, max_len); */
    /* network_close(url); */

    cbm_open(LFN,DEV,SAN,url);
    count = cbm_read(LFN,buffer,max_len);
    cbm_close(LFN);
    
    return count>0;
}

/* /// @brief Retrieve a json parsed url response */
/* /// @param url  */
/* /// @return response length */
/* int getJsonResponse(char *url, char *buffer, int max_len) { */
/*   static int len; */
  
/*   #ifdef USE_VICE_EMULATOR */
/*   static int i; */
  
/*   // Write command file (currently just the url ) */
/*   cbm_open(LFN,11,1,"vice-out"); */
/*   cbm_write(LFN, url, strlen(url)); */
/*   cbm_close(LFN); */

/*   // Wait until command file no longer exists (signifies a response is ready) */
/*   buffer[0]=0; */
/*   i=0; */
/*   while (buffer[0]!=54 && i<24) { */
/*     cbm_open(15,11,15, "r0:vice-out=vice-out"); */
/*     cbm_read(15,buffer, max_len); */
/*     cbm_close(15); */
/*     for (len=0;len<10;len++) { */
/*       waitvsync(); */
/*     } */
/*     i++; */
/*   } */

/*   // If we timed out, return false */
/*   if (i>=24) */
/*     return 0; */

/*   // Read the response into the buffer */
/*   cbm_open(LFN,11,0,"vice-in"); */
/*   len = cbm_read(LFN,buffer, max_len); */
/*   cbm_close(LFN); */

/*   #else */
/*     len=0; */

/*     if (cbm_open(LFN,DEV,SAN,url)) */
/*       return len; */
    
/*     if (cbm_open(CMD,DEV,CMD,"")) { */
/*       cbm_close(LFN); */
/*       return len; */
/*     } */

/*     if (cbm_write(CMD,cmd_parse,sizeof(cmd_parse))>0) { */
/*       if (cbm_write(CMD,json_query,sizeof(json_query))>0) { */
/*         len = cbm_read(LFN,buffer,max_len); */
/*       } */
/*     } */

/*     cbm_close(LFN); */
/*     cbm_close(CMD); */
    
/*   #endif */

/*   return len; */
/* } */

#endif /* __C64__ */
