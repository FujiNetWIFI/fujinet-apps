#ifdef _CMOC_VERSION_

/*
  Network functionality
*/
//#include <apple2.h>
//#include <string.h>
//#include <conio.h>
//#include <stdbool.h>
#include <cmoc.h>

#include "../platform-specific/appkey.h"
#include <fujinet-fuji.h>
#include <fujinet-network.h>
//#include <stdio.h>
#include "../misc.h"

static uint8_t trans_type = OPEN_TRANS_CRLF;
static uint8_t err = 0;

void handle_err(char *message) {
	if (err <  0) {
		printf("err: %s\n", message);
		exit(1);
	}
}
/// @brief Retrieve a json parsed url response
/// @param url 
/// @return response length
uint16_t getJsonResponse(char *url, unsigned char *buffer, uint16_t max_len) {
  int count;
  static char buf[128];
  uint8_t c, err;
  //char * new_url = "N:http://fujinet.online:8080";
  //const char * req_fmt = ;
  //char req[128];
#if 0
  static uint16_t count, bw;
  printf("URL: %s\n", url);
printf("network open\n");
  network_open(url, OPEN_MODE_RW, trans_type);
//  network_json_parse(url);
//  net_set_channel_mode(0,0x01);
printf("back from network open(press enter)\n");
readline();    

printf("network_status\n");
//  for (i=0;i<4;i++)
  {
  //  sprintf(req, "/view?platform=atari&pagesize=1&page=%d", i);
    buf[0]=0;
    sleep(1);
    network_status(url,&bw,&c,&err);
    if (bw<sizeof(buf)) 
    {
      count=bw;
    }
    else
    {
      count= sizeof(buf);
    }
printf("back from network network_status(press enter)\n");
readline();    
//getchar();    
printf("network_read\n");
    n = network_read(url,buf,count);
    if (n < 0) 
    {
        err = -n;
        handle_err("simple_get");
    }
  //  count = network_json_query(new_url, req, buffer);
    printf("count = %d, %s\n", count, buf);
printf("back from network read(press enter)\n");
readline();    
  }
//getchar();    
printf("network close\n");
  network_close(url);
printf("back from network close(press enter)\n");
readline();    
#else
  uint16_t bw;

        network_open(url,12,0);
        network_status(url,&bw,&c,&err);
        network_read(url,buf,bw);
        network_close(url);

#endif  
  return count;
}

#endif

