#include <conio.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "edit_string.h"

#if defined(__ATARI__)

#define FNK_ESC 0x1B
#define FNK_ENTER 0x9B
#define FNK_BS 0x7E
#define FNK_DEL 0xFE
#define FNK_INS 0xFF
#define FNK_LEFT 0x1E
#define FNK_RIGHT 0x1F
#define FNK_HOME 0x01
#define FNK_END 0x05
#define FNK_KILL 0x0B
#define FNK_ASCIIL 0x20
#define FNK_ASCIIH 0x7D

#elif defined(__APPLE2__)

#define FNK_ESC 0x1B
#define FNK_ENTER 0x0D
#define FNK_BS 0x08
#define FNK_DEL 0x7F
// no 'insert', use 0x09 ctrl-I (also tab)
#define FNK_INS 0x09
#define FNK_LEFT 0x08
#define FNK_RIGHT 0x15
#define FNK_HOME 0x01
#define FNK_END 0x05
#define FNK_KILL 0x0B
#define FNK_ASCIIL 0x20
#define FNK_ASCIIH 0x7D

#endif

char kb_get_c() {
    char c = 0;
    c = kbhit();
    if (c == 0) return 0;

    return cgetc();
}

void display_string(uint8_t x, uint8_t y, char* str, int viewport_width, int cursor_pos, bool is_password) {
    int i, char_index, len, start_pos, half_viewport;

    len = strlen(str);
    start_pos = 0;
    half_viewport = viewport_width / 2;

    if (cursor_pos > half_viewport && len >= viewport_width) {
        start_pos = cursor_pos - half_viewport;
        if (cursor_pos >= len) {
            start_pos = len - viewport_width + 1; // Adjust to show the cursor at the end
        } else if (start_pos + viewport_width > len) {
            start_pos = len - viewport_width; // Prevent start_pos from going too far
        }
    }

    gotoxy(x, y);
    for (i = 0; i < viewport_width; i++) {
        char_index = i + start_pos;
        if (char_index == cursor_pos) {
            revers(1); // Invert the character for cursor position
        }

        if (char_index < len) {
            if (!is_password)
                cputc(str[char_index]);
            else
                cputc('*');
        } else {
            cputc(' ');
        }
        if (char_index == cursor_pos) {
            revers(0); // Revert to normal video mode after printing the cursor character
        }
    }
}

bool edit_string(char* initial_str, int max_length, uint8_t x, uint8_t y, uint8_t viewport_width, bool is_password) {
    // cursor_pos is 0 index based position in the string, same as characters would be.
    int cursor_pos, current_length;
    char ch;
    char* buffer = (char*)malloc(max_length + 1);
    memcpy(buffer, initial_str, strlen(initial_str) + 1);
    current_length = strlen(buffer);
    cursor_pos = current_length;
    if (cursor_pos == max_length) cursor_pos--;

    display_string(x, y, buffer, viewport_width, cursor_pos, is_password);

    for (;;) {
        ch = kb_get_c();
        if (ch == 0) continue;

        if (ch == FNK_ENTER) {
            strcpy(initial_str, buffer);
            free(buffer);
            return true;
        } else if (ch == FNK_ESC) {
            free(buffer);
            return false;
        } else if (ch >= FNK_ASCIIL && ch <= FNK_ASCIIH) {
            if (current_length < max_length) {
                buffer[cursor_pos] = ch;
                if (cursor_pos == current_length) {
                    current_length++;
                    buffer[current_length] = '\0';
                }
                if (cursor_pos < (max_length - 1)) {
                    cursor_pos++;
                }
            } else if (current_length == max_length) {
                buffer[cursor_pos] = ch;
                if (cursor_pos < (max_length - 1)) {
                    cursor_pos++;
                }
            }
        } else if (ch == FNK_LEFT) {
            if (cursor_pos > 0) cursor_pos--;
        } else if (ch == FNK_RIGHT) {
            if (cursor_pos < (current_length - 1) || (cursor_pos == (current_length - 1) && current_length < max_length)) {
                cursor_pos++;
            }
        } else if (ch == FNK_DEL) {
            if (cursor_pos < current_length) {
                memmove(&buffer[cursor_pos], &buffer[cursor_pos + 1], current_length - cursor_pos);
                current_length--;
            }
        } else if (ch == FNK_BS) {
            if (cursor_pos > 0) {
                cursor_pos--;
                memmove(&buffer[cursor_pos], &buffer[cursor_pos + 1], current_length - cursor_pos);
                current_length--;
            }
        } else if (ch == FNK_INS) {
            if (cursor_pos < current_length && current_length < max_length) {
                memmove(&buffer[cursor_pos + 1], &buffer[cursor_pos], current_length - cursor_pos + 1);
                buffer[cursor_pos] = ' ';
                current_length++;
            } else if (cursor_pos < current_length) {
                memmove(&buffer[cursor_pos + 1], &buffer[cursor_pos], max_length - cursor_pos - 1);
                buffer[cursor_pos] = ' ';
            }
        } else if (ch == FNK_KILL) {
            buffer[cursor_pos] = '\0';
            current_length = cursor_pos;
        } else if (ch == FNK_HOME) {
            cursor_pos = 0;
        } else if (ch == FNK_END) {
            cursor_pos = current_length;
            if (cursor_pos == max_length) cursor_pos--;
        }

        display_string(x, y, buffer, viewport_width, cursor_pos, is_password);
    }
}
