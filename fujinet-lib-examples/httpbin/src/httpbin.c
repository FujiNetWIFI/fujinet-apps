#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "fujinet-network.h"

#include "httpbin.h"
#include "version.h"

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
#ifdef __CBM__
// #define TRANS_MODE_TEXT OPEN_TRANS_PET
#define TRANS_MODE_TEXT OPEN_TRANS_NONE
#else
#define TRANS_MODE_TEXT OPEN_TRANS_NONE
#endif
uint8_t trans_type_text = TRANS_MODE_TEXT;

uint8_t trans_type_binary = OPEN_TRANS_NONE;

void debug() {}

int main(void) {
    clrscr();
    gotox(0);
    printf("httpbin %s\n", get_version());
    printf("Base URL: %s\n", httpbin);

    setup();

    start_get();                        // save us having to keep closing/reopening.
    test_get_query("");                 // returns entire json *object* line by line. forces you to know structure if you use this (looking at you lobby)

    // example that does match
    test_get_query("/headers/host");    // returns value from httpbin.org

    // examples that return nothing as the json path doesn't match output
    test_get_query("/");                // returns nothing, not the entire root.
    test_get_query("/foo/bar");         // path doesn't exist, returns nothing

    end_get();                          // finally close resource

    // examples of other protocols
    test_post();
    test_put();
    test_delete();

    test_simple_get();

    test_redir_headers();

    if (doesclrscrafterexit()) {
        printf("Press a key to exit.");
        cgetc();
        printf("\n");
    }

    return 0;
}

void setup() {
    uint8_t init_r = 0;
    bzero(url_buffer, 128);
    bzero(result, 1024);
    gotox(0);
    init_r = network_init();
    printf("init: %d, derr: %d\n", init_r, fn_device_error);
  #ifdef __APPLE2__
    printf("nw: %d\n", sp_network);
  #endif
}

void start_get() {
    url = create_url("get");
    err = network_open(url, OPEN_MODE_HTTP_GET, trans_type_text);
    handle_err("open");

    err = network_json_parse(url);
    handle_err("parse");
}

void test_get_query(char *path) {
    int n = 0;
    n = network_json_query(url, path, result);
    if (n < 0) {
        err = -n;
        handle_err("query");
    }

    printf("j: >%s<, r: >%s<\n", path, result);
}

void end_get() {
    err = network_close(url);
    handle_err("close json");
}

// -------------------------------------------------------------------------------
// POST JSON, get result
void test_post() {
    int n = 0;
    url = create_url("post");
    err = network_open(url, OPEN_MODE_HTTP_POST, trans_type_text);
    handle_err("post:open");

    set_json(url);
    network_http_post(url, "{\"name\":\"fenrock\"}");
    err = network_json_parse(url);
    handle_err("post:json parse");
    n = network_json_query(url, "/json/name", result);
    if (n < 0) {
        err = -n;
        handle_err("post:json query");
    }

    printf("/post   :  name=>%s<\n", result);
    err = network_close(url);
    handle_err("post:close");
}

// -------------------------------------------------------------------------------
// PUT JSON, get result. PUT is just a slightly different POST
void test_put() {
    int n = 0;
    url = create_url("put");
    err = network_open(url, OPEN_MODE_HTTP_PUT_H, trans_type_text);
    handle_err("put:open");

    set_json(url);
    network_http_put(url, "{\"level\":11}");
    err = network_json_parse(url);
    handle_err("put:json parse");

    n = network_json_query(url, "/json/level", result);
    if (n < 0) {
        err = -n;
        handle_err("put:json query");
    }

    printf("/put    : level=>%s<\n", result);
    err = network_close(url);
    handle_err("put:close");
}

// -------------------------------------------------------------------------------
// DELETE - no data to send, but response will have data in it
void test_delete() {
    int n = 0;
    url = create_url("delete");
    err = network_http_delete(url, trans_type_text);
    handle_err("del:open");

    set_json(url);
    err = network_json_parse(url);
    handle_err("del:json parse");
    n = network_json_query(url, "/headers/host", result);
    if (n < 0) {
        err = -n;
        handle_err("del:json query");
    }
    printf("/delete :  host=>%s<\n", result);
    err = network_close(url);
    handle_err("del:close");
}

// -------------------------------------------------------------------------------
// GET - no data to send, but response will have data in it
void test_simple_get() {
    int n = 0;
    url = create_url("get");
    // trans mode doesn't appear to be working, always coming back 0x9b on atari
    err = network_open(url, OPEN_MODE_HTTP_GET, trans_type_text);
    handle_err("open");

    // simply read without any fancy modes. FN resets the modes to normal body after closing a connection, so we don't even have to specify the BODY mode.
    // We are only going to read first 40 bytes of the results so we can display it easily.
    // TODO: block reading so very large payloads can be read into limited memory are not yet handled, and above 512 may be broken on apple2.
    n = network_read(url, (uint8_t *) result, 40);
    if (n < 0) {
        err = -n;
        handle_err("simple_get");
    }
    printf("simple read (same as GET):\n");
    hex_dump(result, 40);

    err = network_close(url);
    handle_err("del:close");
}

// use a redirect with PNG return to test header content types and locations
void test_redir_headers() {
    int n = 0;
    uint16_t header_length;

    // use httpbin's /redirect-to AND its /image/png functionality to redirect our request to a PNG image.
    // This should set the "location" header so we can see where we went to, and the content-type header to see it's finally a image/png with a 200 status code
    char *redirect_to_png = "redirect-to?url=https%3A%2F%2Fhttpbin.org%2Fimage%2Fpng&status_code=200";

    url = create_url(redirect_to_png);
    printf("\nFull URL: %s\n", url);

    // must use OPEN_MODE_HTTP_GET_H to return headers, NOTE THE EXTRA "_H"
    err = network_open(url, OPEN_MODE_HTTP_GET_H, trans_type_binary);
    handle_err("open");

    // first specify the headers we are interested in
    begin_collect_headers(url);
    add_header_to_collect(url, "content-type");
    add_header_to_collect(url, "location");
    end_collect_headers(url);

    // now perform the request, just get the header 4 bytes, should be hex 0x89, 0x50, 0x4E, 0x47, which is 0x89 followed by "PNG"
    printf("\nFile first 4 bytes:\n");
    debug();
    n = network_read(url, (uint8_t *) result, 4);
    hex_dump(result, 4);

    // and fetch the values of the headers we were interested in
    err = network_http_set_channel_mode(url, HTTP_CHAN_MODE_GET_HEADERS);
    handle_err("get headers");

    printf("\nHeaders will match:\n  1. content-type\n  2. location\n\n");
    do {
        header_length = get_header_length(url);
        if (header_length > 0) {
            printf("fetching header, size: %d\n", header_length);
            n = network_read(url, (uint8_t *) result, header_length);
            if (n < 0) {
                err = (-n) && 0xFF;
                handle_err("reading header");
            }
            // if the last char is 0x9b, convert to nul char for simple string
            if (result[n - 1] == '\x9b') {
                result[n - 1] = '\0';
            }
            printf("H: >%s<\n", result);
        }
    } while (header_length > 0);
    printf("\n");

    network_close(url);
}


void handle_err(char *reason) {
    if (err) {
        printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
        network_close(url);
        if (doesclrscrafterexit()) {
            cgetc();
        }
        exit(1);
    }
}

char *create_url(char *method) {
    sprintf(url_buffer, "%s%s", httpbin, method);
    return (char *) url_buffer;
}

void set_json(char *devicespec) {
    network_http_start_add_headers(devicespec);
    network_http_add_header(devicespec, "Accept: application/json");
    network_http_add_header(devicespec, "Content-Type: application/json");
    network_http_end_add_headers(devicespec);
}

void begin_collect_headers(char *devicespec) {
    err = network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_COLLECT_HEADERS);
    handle_err("collect headers channel mode");
}

void end_collect_headers(char *devicespec) {
    err = network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
    handle_err("end collect headers");
}

void add_header_to_collect(char *devicespec, char *header) {
    err = network_http_add_header(devicespec, header);
    handle_err("add header");
}

uint16_t get_header_length(char *devicespec) {
    err = network_status(devicespec, &conn_bw, &connected, &conn_err);
    handle_err("header length");
    return conn_bw;
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