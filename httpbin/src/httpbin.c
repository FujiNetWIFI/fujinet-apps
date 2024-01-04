#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "fujinet-network.h"

#include "httpbin.h"

/* 
 * This is an example of doing JSON queries to httpbin.org, and then doing post/put/delete protocol
 * A json query is the same as a network read, except the "mode" has been set to json before issuing the read, so that the FN device can
 * internally parse the results as values to return, rather than the entire body of the response.
*/

char *httpbin = "n:https://httpbin.org/";
char url_buffer[128];
char result[1024];
uint8_t err = 0;
char *url;
uint16_t conn_bw;
uint8_t connected;
uint8_t conn_err;

char *version = "v1.3.7";

void debug() {}

int main(void) {
    printf("httpbin %s\n", version);
    printf("Base URL: %s\n", httpbin);

    setup();

    start_get();                        // save us having to keep closing/reopening.
    // test_get_query("");                 // returns entire json *object* line by line. forces you to know structure if you use this (looking at you lobby)

    // examples that return nothing as the json path doesn't match output
    // test_get_query("/");                // returns nothing, not the entire root.
    test_get_query("/foo/bar");         // path doesn't exist, returns nothing

    // example that does match
    debug();
    test_get_query("/headers/host");    // returns value from httpbin.org

    end_get();                          // finally close resource

    // examples of other protocols
    test_post();
    test_put();
    test_delete();
    test_simple_get();

    printf("Press a key to exit.");
    cgetc();
    printf("\n");

    return 0;
}

void setup() {
    uint8_t init_r = 0;
    bzero(url_buffer, 128);
    bzero(result, 1024);
    gotox(0);
    init_r = network_init();
    printf("init: %d, derr: %d\n", init_r, fn_device_error);
  #ifdef BUILD_APPLE2
    printf("nw: %d\n", sp_network);
  #endif
}

void start_get() {
    url = create_url("get");
    err = network_open(url, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
    handle_err("open");

    err = network_json_parse(url);
    handle_err("parse");
}

void test_get_query(char *path) {
    err = network_json_query(url, path, result);
    handle_err("query");

    printf("j: >%s<, r: >%s<\n", path, result);
}

void end_get() {
    err = network_close(url);
    handle_err("close json");
}

// -------------------------------------------------------------------------------
// POST JSON, get result
void test_post() {
    url = create_url("post");
    err = network_open(url, OPEN_MODE_HTTP_POST, OPEN_TRANS_NONE);
    handle_err("post:open");

    set_json(url);
    network_http_put(url, "{\"name\":\"fenrock\"}");
    err = network_json_parse(url);
    handle_err("post:json parse");
    network_json_query(url, "/json/name", result);
    handle_err("post:json query");
    printf("/post   :  name=>%s<\n", result);
    network_close(url);
    handle_err("post:close");
}

// -------------------------------------------------------------------------------
// PUT JSON, get result. PUT is just a slightly different POST
void test_put() {
    url = create_url("put");
    err = network_open(url, OPEN_MODE_HTTP_PUT_H, OPEN_TRANS_NONE);
    handle_err("put:open");

    set_json(url);
    network_http_put(url, "{\"level\":11}");
    err = network_json_parse(url);
    handle_err("put:json parse");
    network_json_query(url, "/json/level", result);
    handle_err("put:json query");
    printf("/put    : level=>%s<\n", result);
    network_close(url);
    handle_err("put:close");
}

// -------------------------------------------------------------------------------
// DELETE - no data to send, but response will have data in it
void test_delete() {
    url = create_url("delete");
    err = network_http_delete(url, OPEN_TRANS_NONE);
    handle_err("del:open");

    set_json(url);
    err = network_json_parse(url);
    handle_err("del:json parse");
    network_json_query(url, "/headers/host", result);
    handle_err("del:json query");
    printf("/delete :  host=>%s<\n", result);
    network_close(url);
    handle_err("del:close");
}

// -------------------------------------------------------------------------------
// GET - no data to send, but response will have data in it
void test_simple_get() {
    url = create_url("get");
    // trans mode doesn't appear to be working, always coming back 0x9b
    err = network_open(url, OPEN_MODE_HTTP_GET, OPEN_TRANS_LF);
    handle_err("open");

    // simply read without any fancy modes. FN resets the modes to normal body after closing a connection, so we don't even have to specify the BODY mode.
    // We are only going to read first 40 bytes of the results so we can display it easily.
    // TODO: block reading so very large payloads can be read into limited memory are not yet handled, and above 512 may be broken on apple2.
    err = network_read(url, result, 40);
    handle_err("simple_get");
    printf("simple read (same as GET):\n");
    hex_dump(result, 40);

    network_close(url);
    handle_err("del:close");
}

void handle_err(char *reason) {
    if (err) {
        printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
        cgetc();
        exit(1);
    }
}

char *create_url(char *method) {
    sprintf(url_buffer, "%s%s", httpbin, method);
    return (char *) url_buffer;
}

void post(char *devicespec, char *data) {
    set_json(url);
    network_http_post(devicespec, data);
}

void body(char *devicespec, char *r, uint16_t len) {
    strcpy(r, "NO DATA");
    err = network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
    handle_err("body chan mode");
    err = network_read(devicespec, (uint8_t *) r, len);
    handle_err("body read");
}

void set_json(char *devicespec) {
    network_http_start_add_headers(devicespec);
    network_http_add_header(devicespec, "Accept: application/json");
    network_http_add_header(devicespec, "Content-Type: application/json");
    network_http_end_add_headers(devicespec);
}

void hex_dump(char* data, int size) {
    int i = 0;
    int j = 0;
    int p = 0;
    int start = 0;
    int padding = 0;
    char c;

    for (i = 0; i < size; i++) {
        printf("%02x ", (unsigned char)data[i]);

        if ((i + 1) % 8 == 0 || i == size - 1) {
            padding = ((i + 1) % 8) ? (8 - (i + 1) % 8) : 0;
            for (p = 0; p < padding; p++) {
                printf("   "); // for alignment
            }
            printf(" | ");
            start = i - (i % 8);
            for (j = start; j <= i; j++) {
                c = data[j];
                if (isprint((unsigned char)c)) {
                    printf("%c", c);
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
    }
}