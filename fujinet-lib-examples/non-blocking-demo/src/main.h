#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

void configure_animations();
void show_messages(uint8_t modulus);
void handle_err(char *reason);
char* createQuotesUrl();
void draw_border();
void handle_keyboard();

#endif