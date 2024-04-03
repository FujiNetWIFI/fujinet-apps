#ifndef JSONTEST_H
#define JSONTEST_H

#include <stdint.h>
#include <stdbool.h>

void handle_err(char *reason);
void setup();

void start_get();
void end_get();

void test_get_query(char *path);
void test_iss_json();
void test_5cs_json();
void test_httpbin_json();
bool ask();

#endif