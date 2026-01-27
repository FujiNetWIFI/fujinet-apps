#include <cmoc.h>
#include <coco.h>
#include "globals.h"
#include "search.h"
#include "articles.h"
#include "article.h"
#include "cocotext.h"

uint8_t trip=0;  // if trip=1, fujinet is asking us for attention.

void init(void)
{
    initCoCoSupport();
    textMode = getTextMode();
    if (textMode == 32)
    {
        // Use hires text mode by default 
        set_text_width(42);
    }
    else
    {
        set_text_width(textMode);
    }
}

int main(void)
{
    clear_screen(1);
    init();

    while(1)
    {
        switch(state)
        {
        case SEARCH:
            state=search();
            break;
        case ARTICLES:
            state=articles();
            break;
        case ARTICLE:
            state=article();
            break;
        case BYE:
            break;
        }

        if (state==BYE)
            break;
    }

    hirestxt_close();
    clear_screen(1);
    coldStart();
    return 0;
}
