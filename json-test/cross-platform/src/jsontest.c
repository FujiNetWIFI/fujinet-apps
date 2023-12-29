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

char result[1024];
uint8_t err = 0;
char *url;
uint16_t conn_bw;
uint8_t connected;
uint8_t conn_err;

char *version = "v2.0.2";

int main(void) {
    setup();

    printf("jsontest %s\n", version);
    printf("Base URL: %s\n\n", iss_now);

    start_get();                        // save us having to keep closing/reopening.
    test_get_query(longitude_query);
    test_get_query(latitude_query);
    test_get_query(timestamp_query);
    end_get();                          // finally close resource

    printf("press a key to exit.");
    cgetc();

   return 0;
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
    url = iss_now;
    bzero(result, 1024);
    gotox(0);
}

void start_get() {
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
    err = network_json_query(url, path, result);
    handle_err("query");

    printf("  path: %s\nresult: %s\n\n", path, result);
}