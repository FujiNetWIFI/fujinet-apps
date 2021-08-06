#include <stdio.h>
#include <string.h>
#include <atari.h>
#include "app_key.h"
#include "nsio.h"

#define URL "N:HTTP://www.billsgames.com/fujinet/cookies/index.php"

/* See https://github.com/FujiNetWIFI/fujinet-platformio/wiki/HTTP-Protocol */
#define HTTP_OPEN_GET 4
#define HTTP_OPEN_PROPFIND 6
#define HTTP_OPEN_PUT 8
#define HTTP_OPEN_GET_WITH_HEADERS 12
#define HTTP_OPEN_POST 13

/* See https://github.com/FujiNetWIFI/fujinet-platformio/wiki/HTTP-Set-Channel-Mode */
#define HTTP_CHAN_MODE_BODY 0
#define HTTP_CHAN_MODE_COLLECT_HEADERS 1
#define HTTP_CHAN_MODE_GET_HEADERS 2
#define HTTP_CHAN_MODE_SET_HEADERS 3
#define HTTP_CHAN_MODE_POST_SET_DATA 4


/* Some space to store strings (on heap) */
char str[121];
char data[126];
char results[256];


void xio(int iccmd, unsigned char chan, unsigned char aux1, unsigned char aux2, char * filespec) {
  /* FIXME: Do it */
}

void input(unsigned char chan, char * dest_buf) {
  int data_len;

  nstatus(chan);
  data_len = (OS.dvstat[1] << 8) + OS.dvstat[0];
  nread(chan, dest_buf, data_len);
}

void save_cookies(char * cookies) {
  /* FIXME: Do it */
}

char * load_cookies(void) {
  /* FIXME: Do it */
  return "UNSET=NOTHING";
}


void wait_for_keypress(void) {
  printf("Press a key...");
  OS.ch = 255;
  do { } while(OS.ch == 255);
  OS.ch = 255;
}


/* Part 1 - POST data to webserver & get a cookie and page body back */
void part_1(void) {
  /* 1. Ask user for some input */
  printf("Enter some text for the test web server\n");
  printf("to send back as a cookie:\n");
  scanf("%s", str);

  /* 2. Send the data to the website: */
  nopen(1, URL, HTTP_OPEN_POST);
  xio(77, 1, HTTP_OPEN_POST, HTTP_CHAN_MODE_POST_SET_DATA, "N:");
  sprintf(data, "data=%s", str);
  nwrite(1, data, strlen(data));

  /* 3. Get the resulting page's cookie header */
  /* (Step 1: Ask to FujiNet N: device to collect that particular header) */
  xio(77, 1, HTTP_OPEN_POST, HTTP_CHAN_MODE_COLLECT_HEADERS, "N:");
  sprintf(data, "Cookie");
  nwrite(1, data, strlen(data));

  /* (Step 2: Read the header we asked for) */
  input(1, results);

  /* 4. Store the cookie */
  /* (See https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Cookie) */
  save_cookies(results);

  /* 5. Get the resulting page's body */
  strcpy(results, "NO DATA"); /* (just in case!) */
  xio(77, 1, HTTP_OPEN_POST, HTTP_CHAN_MODE_BODY, "N:");
  input(1, results);

  nclose(1);

  /* 6. Show the user the results */
  printf("The webserver responded with:\n%s\n\n", results);

  wait_for_keypress();
}


/* Part 2 - GET data from webserver, sending our cookie */
void part_2(void) {
  /* FIXME */
}

void main(void) {
  printf("%cCookies tester for #FujiNet\n\n", CH_CLR);

  /* FIXME: Ask user which step to take, rather than jump straight into part 1 */

  part_1();
  part_2();

  printf("Okay, stopping here... hit [Reset] or something");
  do { } while(1);
}

