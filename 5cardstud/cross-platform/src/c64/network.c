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
#include "../misc.h"

#define LFN 2     // The logical file number to use for I/O
#define DEV 16    // The network device #
#define SAN 2     // The secondary address (SA) to use on DEV.
#define CMD 15    // The secondary address for the Command channel


#ifndef USE_EMULATOR
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
#else

int getResponse(char *url, unsigned char *buffer, uint16_t max_len)
{
  int16_t count;
  int8_t  i, wait;

  // Delete existing file
  cbm_open(15,11,15, "s:vice-in"); 

  // Write command file (currently just the url )
  cbm_open(LFN,11,1,"vice-out"); 
  cbm_write(LFN, url, strlen(url)); 
  cbm_close(LFN);

  // Wait until command file no longer exists (signifies a response is ready) */
  buffer[0]=0; 
  wait=0; 
  count = 1;
  while (count != 0 ) {

    // If we timed out (>15 seconds), return false 
    wait++; 
    if (wait > 90) {
      return 0;
    }

    // Short delay
    for (i=0;i<10;i++) { 
      waitvsync(); 
    } 

    // Check if file exists by renaming it
    count = cbm_open(15,11,15, "r0:vice-out=vice-out"); 
    cbm_close(15); 
  } 

  // Read the response into the buffer 
  cbm_open(LFN,11,0,"vice-in"); 
  count = cbm_read(LFN,buffer, max_len); 
  cbm_close(LFN); 

  return count>0;
}
#endif


#endif /* __C64__ */
