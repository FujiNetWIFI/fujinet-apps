#ifndef NET_READ_H
#define NET_READ_H

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

// these will be overridden by make
#ifndef REST_SERVER_ADDRESS
#define REST_SERVER_ADDRESS "localhost"
#endif

// these will be overridden by make
#ifndef REST_SERVER_PORT
#define REST_SERVER_PORT "8080"
#endif

char *createAbUrl(int num);
char *createCabUrl(int num);
void handle_err(char *reason);
void clear_buffer();
void hex_dump(uint8_t* data, int size);
void new_screen();
void do_alpha(int num);
void do_chunked(int total, int size);
void do_multi(int total, int size);
void do_read(int num);
void do_google();

#endif