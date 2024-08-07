#ifndef HTTPBIN_H
#define HTTPBIN_H

#include <stdint.h>

void add_header(char *devicespec, char *header);
char *create_url(char *method);
void handle_err(char *reason);
uint8_t set_http_channel_mode(char *devicespec, uint8_t mode);
void set_json(char *devicespec);
void setup();

void start_get();
void end_get();

void body(char *devicespec, char *r, uint16_t len);

void test_get_query(char *path);
void test_delete();
void test_post();
void test_put();
void test_simple_get();
void test_redir_headers();
void hex_dump(char* data, int size);

void begin_collect_headers(char *devicespec);
void end_collect_headers(char *devicespec);
void add_header_to_collect(char *devicespec, char *header);
uint16_t get_header_length(char *devicespec);

#ifdef __APPLE2__
extern uint8_t sp_network;
#endif

#endif