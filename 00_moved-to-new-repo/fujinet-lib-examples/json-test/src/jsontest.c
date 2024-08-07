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

char chuck[] = "n:https://api.chucknorris.io/jokes/random";
char chuck_query[] = "/value";

char iss_now[] = "n:http://api.open-notify.org/iss-now.json";
char longitude_query[]  = "/iss_position/longitude";
char latitude_query[]   = "/iss_position/latitude";
char timestamp_query[]  = "/TIMESTAMP";

// Not getting data from this anymore
// char five_card_stud_url[] = "n:https://5card.carr-designs.com/state?table=blue&player=THOMC";
// char round_query[] = "/round";
// char pot_query[] = "/pot";

char httpbin_url[] = "n:https://httpbin.org/get";
char host_query[] = "/headers/host";

// These are not JSON tests, but html pages that were failing to parse in fujinet-pc client due to missing content-length / or some weird chunk processing by mongoose after redirect
char fuji_online_http[] = "n:http://fujinet.online/fujifind/index.php?q=fujinet";
char fuji_online_https[] = "n:https://fujinet.online/fujifind/index.php?q=fujinet";

char result[1024];
uint8_t err = 0;
char *url;
uint16_t conn_bw;
uint8_t connected;
uint8_t conn_err;

bool sure = false;

char *version = "v2.2.0";

int main(void) {
    setup();

    printf("jsontest %s\n", version);

    test_chuck();
    test_iss_json();
    test_httpbin_json();
    // test_5cs_json();
    test_fuji_http();
    test_fuji_https();

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

void test_chuck() {
    printf("Chuck Norris Test\n");
    sure = ask();
    printf("\n");
    if (!sure) return;

    open_and_parse_json(chuck);
    json_query(chuck_query);
    close();
}

void test_iss_json() {
    printf("ISS JSON Test\n");
    sure = ask();
    printf("\n");
    if (!sure) return;

    open_and_parse_json(iss_now);
    json_query(longitude_query);
    json_query(latitude_query);
    json_query(timestamp_query);
    close();
}

void test_httpbin_json() {
    printf("Httpbin Test\n");
    sure = ask();
    printf("\n");
    if (!sure) return;

    open_and_parse_json(httpbin_url);
    json_query(host_query);
    close();
}

// void test_5cs_json() {
//     printf("5 Card Stud Test\n");
//     sure = ask();
//     printf("\n");
//     if (!sure) return;

//     open_and_parse_json(five_card_stud_url);
//     json_query(round_query);
//     json_query(pot_query);
//     close();
// }

void test_fuji_http() {
    printf("Fujinet Online HTTP\n");
    sure = ask();
    printf("\n");
    if (!sure) return;

    open(fuji_online_http);
    get();
    close();
}

void test_fuji_https() {
    printf("Fujinet Online HTTPS\n");
    sure = ask();
    printf("\n");
    if (!sure) return;

    open(fuji_online_https);
    get();
    close();
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

void open_and_parse_json(char *u) {
    url = u;
    err = network_open(url, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
    handle_err("open");

    err = network_json_parse(url);
    handle_err("parse");
}

void open(char *u) {
    url = u;
    err = network_open(url, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
    handle_err("open");
}

void close() {
    err = network_close(url);
    handle_err("close");
}

void json_query(char *path) {
    int count = 0;
    count = network_json_query(url, path, result);
    if (count < 0) {
        err = -count;
        handle_err("query");
    }

    printf("  path: %s\nresult: %s\n\n", path, result);
}

void get() {
    int count = 0;
    count = network_read(url, (uint8_t *) result, 120);
    if (count < 0) {
        err = -count;
        handle_err("query");
    }

    printf("  result: %s\n\n", result);
}