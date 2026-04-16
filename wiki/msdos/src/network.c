#include <stdio.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "globals.h"
#include "screen.h"

static char url[256];
static char response_buffer[2048];

static const char *urlBase = "N:HTTPS://fujinet.online/8bitwiki/wiki.php";
static const char *articles_printf_spec = "%s?t=lf&ps=%dx%d&l=%u&p=%u&s=%s";
static const char *article_printf_spec = "%s?t=lf&ps=%dx%d&p=%u&h=1&a=%s";

static void url_encode(char *dest, const char *src)
{
    static const char hex[] = "0123456789ABCDEF";
    unsigned char c;

    while ((c = (unsigned char)*src++) != 0)
    {
        if ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||
            c == '-' || c == '_' || c == '.' || c == '~')
        {
            *dest++ = c;
        }
        else
        {
            *dest++ = '%';
            *dest++ = hex[(c >> 4) & 0x0F];
            *dest++ = hex[c & 0x0F];
        }
    }

    *dest = 0;
}

void setup_url(bool articles)
{
    char tmp_buf[256];
    url_encode(tmp_buf, search_term);

    if (articles)
    {
        sprintf(url, articles_printf_spec,
            urlBase,
            screen_cols,
            rows,
            articles_per_page,
            articles_page,
            tmp_buf);
    }
    else
    {
        sprintf(url, article_printf_spec,
            urlBase,
            screen_cols - 1,
            rows,
            article_page,
            article_id);
    }
}

char *fetch_data(bool articles)
{
    uint16_t bytesWaiting;
    uint8_t connected;
    uint8_t error;
    unsigned int buf_offset = 0;

    setup_url(articles);

    memset(response_buffer, 0, sizeof(response_buffer));

    network_open(url, OPEN_MODE_RW, OPEN_TRANS_NONE);
    network_status(url, &bytesWaiting, &connected, &error);

    while (error == 1 && bytesWaiting > 0)
    {
        network_read(url, (uint8_t *)&response_buffer[buf_offset], bytesWaiting);
        buf_offset += bytesWaiting;
        network_status(url, &bytesWaiting, &connected, &error);

        strcat(fetching_buf, ".");
        show_fetching_msg(false);
    }

    network_close(url);

    return response_buffer;
}
