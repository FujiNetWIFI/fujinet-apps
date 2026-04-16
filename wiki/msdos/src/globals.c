#include "globals.h"
#include <ctype.h>

State state;
int articles_page;
int articles_per_page;
char *article_id;
int article_page;
char fetching_buf[81];
int rows;
char search_term[115];

bool is_numeric(const char *s)
{
    if (*s == '\0')
        return false;

    while (*s)
    {
        if (!isdigit((unsigned char)*s))
            return false;
        s++;
    }
    return true;
}
