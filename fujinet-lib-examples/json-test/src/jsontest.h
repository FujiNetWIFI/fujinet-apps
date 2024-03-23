#ifndef JSONTEST_H
#define JSONTEST_H

#include <stdint.h>

void handle_err(char *reason);
void setup();

void start_get();
void end_get();

void test_get_query(char *path);

#endif