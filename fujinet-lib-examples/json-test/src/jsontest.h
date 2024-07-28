#ifndef JSONTEST_H
#define JSONTEST_H

#include <stdint.h>
#include <stdbool.h>

void handle_err(char *reason);
void setup();

void open_and_parse_json(char *u);
void open();
void close();

void json_query(char *path);
void get();
void test_chuck();
void test_iss_json();
// void test_5cs_json();
void test_httpbin_json();
void test_fuji_http();
void test_fuji_https();
bool ask();

#endif