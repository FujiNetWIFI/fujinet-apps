#include <cc65.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fujinet-network.h>

#include "contiki.h"
#include "www.h"

#include "webclient.h"

PROCESS(webclient_process, "Web Client");

struct webclient_state {
  uint8_t open;
  uint8_t connected;
  char mimetype[32];
  char device[2 + WWW_CONF_MAX_URLLEN + 1];
  char buffer[2048];
};

static struct webclient_state s;

/*-----------------------------------------------------------------------------------*/
char *
webclient_mimetype(void)
{
  return s.mimetype;
}
/*-----------------------------------------------------------------------------------*/
unsigned char
webclient_open(const char *url)
{
  if(s.open) {
    webclient_close();
  }

  strncpy(s.device + 2, url, WWW_CONF_MAX_URLLEN);

  if(network_open(s.device, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE) != FN_ERR_OK) {
    return 0;
  }
  s.open = 1;

  strcpy(s.mimetype, "html");

  webclient_connected();

  process_poll(&webclient_process);
  return 1;
}
/*-----------------------------------------------------------------------------------*/
void
webclient_close(void)
{
  if(!s.open) {
    return;
  }

  network_close(s.device);
  s.open = 0;

  webclient_closed();
}
/*-----------------------------------------------------------------------------------*/
static void
webclient_init(void)
{
  if(network_init() != FN_ERR_OK) {
    puts("Network init error");
    if(doesclrscrafterexit()) {
      puts("Press any key to continue ...");
      ctk_arch_getkey();
    }
    exit(EXIT_FAILURE);
  }
  
  strcpy(s.device, "N:");
}
/*-----------------------------------------------------------------------------------*/
static void
webclient_exit(void)
{
}
/*-----------------------------------------------------------------------------------*/
static void
webclient_poll(void)
{
  uint16_t retval;

  if(!s.open) {
    return;
  }

  retval = network_read_nb(s.device, s.buffer, sizeof(s.buffer));

  if (retval > 0) {
    webclient_datahandler(s.buffer, retval);
  }

  /* Check if we hit EOF, i.e. no more data for this URL */
  if (fn_network_error == 136) {
    network_close(s.device);
    s.open = 0;

    webclient_datahandler(NULL, 0);
    
    return;
  }

  process_poll(PROCESS_CURRENT());
}
/*-----------------------------------------------------------------------------------*/
PROCESS_THREAD(webclient_process, ev, data)
{
  PROCESS_POLLHANDLER(webclient_poll());
  
  PROCESS_BEGIN();

  webclient_init();

  PROCESS_WAIT_UNTIL(ev == PROCESS_EVENT_EXIT);

  webclient_exit();

  PROCESS_END();
}
/*-----------------------------------------------------------------------------------*/
