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
void del(char *devicespec);
void post(char *devicespec, char *data);

void test_get_query(char *path);
void test_delete();
void test_post();
void test_put();
void test_simple_get();
void hex_dump(char* data, int size);

#ifdef BUILD_APPLE2
extern uint8_t sp_network;
#endif

#endif