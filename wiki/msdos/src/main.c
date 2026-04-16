#include <stdlib.h>
#include "globals.h"
#include "screen.h"
#include "search.h"
#include "articles.h"
#include "article.h"

int main(void)
{
    screen_init();

    while (1)
    {
        switch (state)
        {
        case SEARCH:
            state = search();
            break;
        case ARTICLES:
            state = articles();
            break;
        case ARTICLE:
            state = article();
            break;
        case BYE:
            break;
        }

        if (state == BYE)
            break;
    }

    screen_end();
    return 0;
}
