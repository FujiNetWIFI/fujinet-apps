#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include <stdint.h>

#define ARROW_UP    0x80
#define ARROW_DOWN  0x81
#define ARROW_LEFT  0x82
#define ARROW_RIGHT 0x83
#define ENTER       0x0D
#define KEY_ESCAPE  0x1B
#define KEY_BACKSPACE 0x08

typedef enum _state
{
    SEARCH = 0,
    ARTICLES,
    ARTICLE,
    BYE,
} State;

extern State state;
extern int articles_page;
extern int articles_per_page;
extern char *article_id;
extern int article_page;
extern char fetching_buf[];
extern int rows;
extern char search_term[];
extern unsigned char screen_cols;

bool is_numeric(const char *s);

#endif /* GLOBALS_H */
