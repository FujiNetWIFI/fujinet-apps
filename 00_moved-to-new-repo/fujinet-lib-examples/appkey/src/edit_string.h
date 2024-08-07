#ifndef EDIT_STRING_H
#define EDIT_STRING_H

#include <stdbool.h>
#include <stdint.h>

bool edit_string(char* initial_str, int max_length, uint8_t x, uint8_t y, uint8_t viewport_width, bool is_password);

#endif // EDIT_STRING_H