#ifdef __C64__
/*
  This is a temporary placeholder for platform specific code until a 
  cross platform solution is created.

  That will be done once the basic skeleton of the game is ported over from FB.
*/

#include <c64.h>
#include <cbm.h>
#include <string.h>
#include <stdbool.h>

#define LFN 2     // The logical file number to use for I/O
#define DEV 16    // The network device #
#define SAN 2     // The secondary address (SA) to use on DEV.
#define CMD 15    // The secondary address for the Command channel

// todo: code for real Meatloaf/FujiNet

// const char cmd_parse[]="jsonparse,2";
// const char json_query[] = "jq,2,/timestamp";
// {
//   cbm_open(CMD,DEV,CMD,"");
//   cbm_open(LFN,DEV,SAN,url);

//   cbm_write(CMD,cmd_parse,sizeof(cmd_parse));
//   cbm_write(CMD,ts_query,sizeof(ts_query));
//   len = cbm_read(LFN,tmp,sizeof(tmp));

//   cbm_close(LFN);
//   cbm_close(CMD);
// }

/// @brief Quick and dirty stub to retrieve a json parsed url response
/// @param url 
unsigned char  getJsonResponse(char *url, unsigned char* buf, int maxLen) {
  unsigned char i,j;
  int bytesRead;

  // Convert letters to Ascii
  i=strlen(url);
  while (i--) {
    j=url[i];
    if (j>=65 && j<=90)
      j+=32;
    else if (j>=193)
      j-=128;
    url[i]=j;
  }

  // Write command file (currently just the url )
  cbm_open(LFN,DEV,1,"vice-out");
  cbm_write(LFN, url, strlen(url));
  cbm_close(LFN);

  // Wait until command file no longer exists (signifies a response is ready)
  buf[0]=0;
  i=0;
  while (buf[0]!=54 && i<24) {
    cbm_open(15,DEV,15, "r0:vice-out=vice-out");
    cbm_read(15,buf, maxLen);
    cbm_close(15);
    for(j=0;j<10;j++)
      waitvsync();

    i++;
  }

  // If we timed out, return false
  if (i>=24)
    return false;

  // Read the response into the buffer
  cbm_open(LFN,DEV,0,"vice-in");
  bytesRead = cbm_read(LFN,buf, maxLen);
  cbm_close(LFN);
  
  return bytesRead=0;
}

void initialize(){}
#endif /* __C64__ */