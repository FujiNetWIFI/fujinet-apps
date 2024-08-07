/**
 * Fujinet lib non-blocking demo.
 * 
 */

// see "net-read/README.md" for information on downloading and running the REST service
// for this application

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "fujinet-network.h"

#include "main.h"
#include "animation.h"
#include "quotes.h"
#include "display.h"

#include "debug.h"

#define BYTES_TO_READ 40

extern uint16_t fn_network_bw;
extern uint8_t fn_network_conn;
extern uint8_t fn_network_error;

uint8_t last_key = 0;
int current_frame = 0;
bool is_connected = false;
uint8_t message_speed = 50;
bool update_debug = true;
bool pause_messages = false;

char quotesUrl[80];
uint8_t buffer[BYTES_TO_READ + 1];

uint8_t err = 0;
char *url;

uint16_t nw_bw = 0;
uint8_t nw_conn = 0;
uint8_t nw_err = 0;

BoxAnimation anim1;
BoxAnimation anim2;
BoxAnimation anim3;
BoxAnimation anim4;

void print_debug()
{
  gotoxy(0, 10);
  cprintf("m: %2d  a1: %2d  a2: %2d  a3: %2d  a4: %2d", message_speed, anim1.modulus, anim2.modulus, anim3.modulus, anim4.modulus);
}

int main(void)
{
  int max_frames = 0;
  uint8_t init_r = 0;
  uint8_t key_hit = 0;

  clrscr();
  configure_animations();

  max_frames = anim1.modulus * anim2.modulus * anim3.modulus * anim4.modulus;

  init_r = network_init();
  if (init_r != 0) {
    err = init_r;
    handle_err("network_init failed");
  }
  url = createQuotesUrl();

  draw_border();
  cputsxy(0, 9, "keys: +/-, q/a, w/s, e/d, r/f, p, ESC");

  // loop until ESC hit
  while(last_key != 27) {
    animate(&anim1);
    animate(&anim2);
    animate(&anim3);
    animate(&anim4);

    if (!pause_messages) {
      show_messages(message_speed);
    }

    if (update_debug) {
      print_debug();
      update_debug = false;
    }

    current_frame++;
    if (current_frame > max_frames) {
      current_frame = 0;
    }

    key_hit = kbhit();
    if (key_hit != 0) {
      last_key = cgetc();
      handle_keyboard(last_key);
    }
  }
  network_close(url);
  cgetc();
  clrscr();
  gotoxy(2, 0);

  return 0;
}


char* createQuotesUrl() {
  // This endpoint produces a single random quote, then the stream is closed, and the application opens a new one
  sprintf(quotesUrl, "n:http://%s:%s/quotes/random/", REST_SERVER_ADDRESS, REST_SERVER_PORT);

  return (char *)quotesUrl;
}

void draw_border()
{
  int x, y;
  for (x = 1; x <= 38; x++) {
    cputcxy(x, 12, '#');
    cputcxy(x, 23, '#');
  }
  for (y = 12; y < 24; y++) {
    cputcxy( 1, y, '#');
    cputcxy(10, y, '#');
    cputcxy(19, y, '#');
    cputcxy(20, y, '#');
    cputcxy(29, y, '#');
    cputcxy(38, y, '#');
  }
}

void handle_err(char *reason) {
  if (err) {
    printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
    cgetc();
    exit(1);
  }
}


void show_messages(uint8_t modulus)
{
  uint8_t i = 0;
  uint8_t j = 0;
  uint16_t n = 0;
  bool more_data = true;

  if ((current_frame % modulus) != 0) {
    return;
  }

  // debug();
  if (!is_connected) {
    err = network_open(url, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
    handle_err("open");
    is_connected = true;
  }

  n = network_read_nb(url, buffer, BYTES_TO_READ);

  // check if we hit EOF, i.e. no more data for this URL
  if (fn_network_error == 136) {
    is_connected = false;
    network_close(url);
    return;
  }

  // check  if there was no data, this is when we requested more but there wasn't any available.
  if (n == 0) return;

  // did we read the entire contents of available data? if not, the quote isn't finished.
  more_data = (fn_network_bw > 0);

  // add end of string terminator
  buffer[n] = 0;

  processBuffer((const char *) &buffer[0], more_data);

}


void configure_animations()
{
  anim1.current = 0;
  anim1.data_len = b1_x_size;
  anim1.modulus = 2;
  anim1.x_data = b1_x;
  anim1.y_data = b1_y;
  anim1.x_offset = 2;
  anim1.y_offset = 13;
  anim1.pixel = 'X';
  anim1.trail_len = 5;

  anim2.current = 0;
  anim2.data_len = b2_x_size;
  anim2.modulus = 2;
  anim2.x_data = b2_x;
  anim2.y_data = b2_y;
  anim2.x_offset = 11;
  anim2.y_offset = 13;
  anim2.pixel = '#';
  anim2.trail_len = 7;

  anim3.current = 0;
  anim3.data_len = b3_x_size;
  anim3.modulus = 2;
  anim3.x_data = b3_x;
  anim3.y_data = b3_y;
  anim3.x_offset = 21;
  anim3.y_offset = 13;
  anim3.pixel = '@';
  anim3.trail_len = 10;

  anim4.current = 0;
  anim4.data_len = b4_x_size;
  anim4.modulus = 2;
  anim4.x_data = b4_x;
  anim4.y_data = b4_y;
  anim4.x_offset = 30;
  anim4.y_offset = 13;
  anim4.pixel = '*';
  anim4.trail_len = 12;

}

void handle_keyboard()
{
  // ctrl-up or '+', go faster by reducing the modulus (which is less delay, so faster)
  if ((last_key == 28 || last_key == 43) && message_speed > 1) {
    message_speed--;
    update_debug = true;
    return;
  }
  // ctrl-down or '-', slower
  if ((last_key == 29 || last_key == 45) && message_speed < 99) {
    message_speed++;
    update_debug = true;
    return;
  }

  // Q, anim1 speed faster 
  if (last_key == 113 && anim1.modulus > 1) {
    anim1.modulus--;
    update_debug = true;
    return;
  }

  // A, anim1 speed slower 
  if (last_key == 97 && anim1.modulus < 99) {
    anim1.modulus++;
    update_debug = true;
    return;
  }

  // W, anim2 speed faster 
  if (last_key == 119 && anim2.modulus > 1) {
    anim2.modulus--;
    update_debug = true;
    return;
  }

  // S, anim2 speed slower 
  if (last_key == 115 && anim2.modulus < 99) {
    anim2.modulus++;
    update_debug = true;
    return;
  }

  // E, anim3 speed faster 
  if (last_key == 101 && anim3.modulus > 1) {
    anim3.modulus--;
    update_debug = true;
    return;
  }

  // D, anim3 speed slower 
  if (last_key == 100 && anim3.modulus < 99) {
    anim3.modulus++;
    update_debug = true;
    return;
  }

  // R, anim4 speed faster 
  if (last_key == 114 && anim4.modulus > 1) {
    anim4.modulus--;
    update_debug = true;
    return;
  }

  // F, anim1 speed slower 
  if (last_key == 102 && anim4.modulus < 99) {
    anim4.modulus++;
    update_debug = true;
    return;
  }

  if (last_key == 112) {
    pause_messages = !pause_messages;
  }

}