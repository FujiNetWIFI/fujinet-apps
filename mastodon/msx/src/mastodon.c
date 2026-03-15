/**
 * @brief   Mastodon Client
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details
 * @verbose {verbose}
 */

#include <stdbool.h>
#include <conio.h>
#include <video/tms99x8.h>
#include <fujinet-network.h>
#include <string.h>
#include <stdlib.h>
#include <sound.h>
#include <psg.h>

// MSX tick counter.
__at (0xFC9E) unsigned int tikcnt;

const char url[]="N:HTTPS://oldbytes.space/api/v1/timelines/public?limit=1";
const char display_name_query[]="/0/account/display_name";
const char created_at_query[]="/0/created_at";
const char content_query[]="/0/content";

char tmp[256];

// Sprite data for Mastodon logo and Cursor
const unsigned char spriteData[9][32] =
    {
        // Sprite 0, color 4, top left
        {0x00,0x03,0x0F,0x1F,0x3F,0x3F,0x7F,0x7E,0x7E,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0xFF,0xFF,0xFF,0xFF,0xFF,0x87,0x01,0x01,0x00,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C},
        // Sprite 1, color F, top left
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x78,0xFE,0xFE,0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3},
        // Sprite 2, color 4, top right
        {0xFF,0xFF,0xFF,0xFF,0xFF,0xE1,0x80,0x80,0x00,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x00,0xE0,0xF0,0xF8,0xFC,0xFC,0xFE,0x7E,0x7E,0x3E,0x3E,0x3E,0x3E,0x3E,0x3E,0x3E},
        // Sprite 3, color F, top right
        {0x00,0x00,0x00,0x00,0x00,0x1E,0x7F,0x7F,0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0},
        // Sprite 4, color 4, bottom left
        {0x7C,0x7C,0x7C,0x7E,0x7F,0x3F,0x3F,0x3F,0x3F,0x3F,0x1F,0x0F,0x07,0x03,0x00,0x00,0x3F,0x3F,0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xC7,0xC0,0xE0,0xF0,0xFF,0xFF,0xFF,0x3F},
        // Sprite 5, color F, bottom left
        {0x03,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        // Sprite 6, color 4, bottom right
        {0xFC,0xFC,0xFC,0xFE,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0xFC,0xFC,0xF8,0xE0,0x3E,0x3E,0x3E,0x7C,0xFC,0xF8,0xF0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        // Sprite 7, color F, bottom right
        {0x03,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        // Sprite 8, color F, cursor
        {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    };

// Is cursor visible?
bool cursorVisible=true;

const char _display_name[32];
const char _date[32];
const char _toot[5120];

void wait_vblank(void)
{
    int t1,t2;
    int i=0;

    t1 = tikcnt;
    while (i<1000) {i++;}
    t2 = tikcnt;

    while (t1==t2);
}

void init(void)
{
    psg_init();
    psg_channels(chanAll,chanNone);

    vdp_set_mode(2);
    vdp_color(3,1,1);
    vdp_set_sprite_mode(sprite_large);
    for (int i=0;i<9;i++)
        vdp_set_sprite_16(i,spriteData[i]);
    vdp_put_sprite_16(0,0,0,0,4);
    vdp_put_sprite_16(1,0,0,1,15);
    vdp_put_sprite_16(2,16,0,2,4);
    vdp_put_sprite_16(3,16,0,3,15);
    vdp_put_sprite_16(4,0,16,4,4);
    vdp_put_sprite_16(5,0,16,5,15);
    vdp_put_sprite_16(6,16,16,6,4);
    vdp_put_sprite_16(7,16,16,7,15);
    add_raster_int(tick_count_isr);
}

void fetch_post(char *display_name, char *date, char *toot)
{
    clrscr();

    gotoxy(0,11);
    msx_color(10,1,1);
    cprintf("    FETCHING NEWEST POST...    ");

    // Play a little sound.
    for (int i=0;i<4;i++)
    {
        psg_volume(1,8);
        psg_tone(1,psgT(i*120));
        wait_vblank();
        wait_vblank();
        psg_volume(1,0);
    }

    memset(date,0,32);
    memset(display_name,0,32);
    memset(toot,0,5120);

    network_init();
    network_open(url,OPEN_MODE_HTTP_GET,0x00); // Strip SGML
    network_json_parse(url);
    network_json_query(url,display_name_query,tmp);
    strncpy(display_name, tmp, 31);
    network_json_query(url,created_at_query,tmp);
    strncpy(date,tmp,31);
    network_json_query(url,content_query,toot);
    network_close(url);
}

void show_toot(const char* display_name, const char* date, const char *toot)
{
    clrscr();

    textcolor(2);

    psg_envelope(envD,psgT(100),chanAll);
    psg_tone(1,psgT(440));
    gotoxy(6,0); cprintf("%24s","Latest Mastodon post");
    gotoxy(6,1); cprintf("%24s","from OLDBYTES.SPACE");

    textcolor(12); gotoxy(0,23); cprintf("%-31s","POST REFRESH EVERY 2 MINUTES");

    tikcnt=0;
    while (tikcnt<20);

    textcolor(3);

    psg_envelope(envD,psgT(100),chanAll);
    psg_tone(1,psgT(320));
    gotoxy(6,3); cprintf("%24s",display_name);
    gotoxy(6,4); cprintf("%24s",date);

    tikcnt=0;
    while(tikcnt<40);

    textcolor(15);

    gotoxy(0,6);

    int in_tag = 0;

    for (int i = 0; i < 512-32; i++)
        if (!toot[i])
            break;
        else
        {
            if (toot[i] == '<') { in_tag = 1; continue; }
            if (toot[i] == '>') { in_tag = 0; continue; }
            if (in_tag) continue;

            tikcnt = 0;
            cputc(toot[i]);
            psg_tone(1,psgT(370));
            psg_volume(1,7);
            vdp_put_sprite_16(8,wherex()*8,wherey()*8,8,5);
            while (tikcnt<1);
            psg_volume(1,0);
            while (tikcnt<2);
        }

    if (strlen(toot) > 512-32)
        cputs("...");
}

void main(void)
{
    int cursorTimer=0;

    init();

    while(1)
    {
        fetch_post(_display_name,_date,_toot);
        show_toot(_display_name,_date,_toot);

        tikcnt=0;
        while(tikcnt<4096)
        {
            wait_vblank();
            cursorTimer++;

            if (cursorTimer == 10)
            {
                cursorVisible = !cursorVisible;
                cursorTimer=0;
            }

            if (cursorVisible)
                vdp_put_sprite_16(8,0xFF,0xFF,8,5);
            else
                vdp_put_sprite_16(8,wherex()*8,wherey()*8,8,5);
        };

        vdp_put_sprite_16(8,0xFF,0xFF,8,5);
    }
}
