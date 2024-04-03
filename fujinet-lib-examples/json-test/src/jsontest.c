/**
 * jsontest.c - network-lib version
 *
 * A test program showing how to use the JSON parsing functions to traverse
 * live data from e.g. HTTPS, using common network library for multiple platforms
 *
 * Mark Fisher <mark dot j dot fisher at gmail dot com>
 * 
 * License: v. 3, see LICENSE.md for details
 *
 * 2023-12-07
 */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "jsontest.h"
#include "fujinet-network.h"

char iss_now[] = "N:HTTP://api.open-notify.org/iss-now.json";
char longitude_query[]  = "/iss_position/longitude";
char latitude_query[]   = "/iss_position/latitude";
char timestamp_query[]  = "/TIMESTAMP";

char five_card_stud_url[] = "n:https://5card.carr-designs.com/state?table=blue&player=THOMC";
char round_query[] = "/round";
char pot_query[] = "/pot";

char httpbin_url[] = "n:https://httpbin.org/get";
char host_query[] = "/headers/host";

char result[1024];
uint8_t err = 0;
char *url;
uint16_t conn_bw;
uint8_t connected;
uint8_t conn_err;

bool sure = false;

char *version = "v2.1.0";

int main(void) {
    setup();

    printf("jsontest %s\n", version);

    test_iss_json();
    test_httpbin_json();
    test_5cs_json();

    printf("press a key to exit.");
    cgetc();

   return 0;
}

bool ask() {
  char sure = 0;
  printf("Run this test? ");
  sure = cgetc();
  if (sure == 'y' || sure == 'Y') {
    return true;
  } else {
    return false;
  }
}

void test_iss_json() {
    printf("ISS JSON Test\n");
    sure = ask();
    if (!sure) return;
    printf("\n");

    start_get(iss_now);
    test_get_query(longitude_query);
    test_get_query(latitude_query);
    test_get_query(timestamp_query);
    end_get();
}

void test_httpbin_json() {
    printf("Httpbin Test\n");
    sure = ask();
    if (!sure) return;
    printf("\n");

    start_get(httpbin_url);
    test_get_query(host_query);
    end_get();
}

void test_5cs_json() {
    printf("5 Card Stud Test\n");
    sure = ask();
    if (!sure) return;
    printf("\n");

    start_get(five_card_stud_url);
    test_get_query(round_query);
    test_get_query(pot_query);
    end_get();
}

void handle_err(char *reason) {
    if (err) {
        printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
        printf("press a key to exit.");
        cgetc();
        exit(1);
    }
}

void setup() {
    bzero(result, 1024);
    gotox(0);
}

void start_get(char *u) {
    url = u;
    err = network_open(url, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
    handle_err("open");

    err = network_json_parse(url);
    handle_err("parse");
}

void end_get() {
    err = network_close(url);
    handle_err("close");
}

void test_get_query(char *path) {
    int count = 0;
    err = network_json_query(url, path, result);
    if (count < 0) {
        err = -count;
        handle_err("query");
    }

    printf("  path: %s\nresult: %s\n\n", path, result);
}