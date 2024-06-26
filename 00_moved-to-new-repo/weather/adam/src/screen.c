/**
 * Weather / screen.c
 *
 * Screen Routines
 */


#include <video/tms99x8.h>
#include <eos.h>
#include <smartkeys.h>
#include <conio.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <ctype.h>
#include "screen.h"
#include "sprite.h"
#include "constants.h"
#include "ftime.h"
#include "io.h"
#include "options.h"
#include "forecast.h"
#include "location.h"

char tmp[192]; // Temporary for screen formatting

unsigned short screen_icon_counter;
extern unsigned char forecast_offset;
extern unsigned char response[1024];

extern OptionsData optData;
extern Location locData;
  
FUJI_TIME current_time;


// udgs = user defined graphics set
const unsigned char udgs[] = {
    // Digits 0-9 as 16x16 chars
    0x03, 0x0F, 0x0C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x0C, 0x0F, 0x03, 0xC0, 0xF0, 0x30, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x30, 0xF0, 0xC0,
    0x01, 0x03, 0x07, 0x07, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x07, 0x07, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xE0,
    0x03, 0x0F, 0x0C, 0x18, 0x18, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0E, 0x1C, 0x18, 0x1F, 0x1F, 0xC0, 0xF0, 0x30, 0x18, 0x18, 0x38, 0x70, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8,
    0x03, 0x0F, 0x0C, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x0C, 0x0F, 0x03, 0xC0, 0xF0, 0x30, 0x18, 0x18, 0x18, 0x30, 0xE0, 0xE0, 0x30, 0x18, 0x18, 0x18, 0x30, 0xF0, 0xC0,
    0x01, 0x01, 0x03, 0x03, 0x06, 0x06, 0x0C, 0x0C, 0x18, 0x18, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xF0, 0xF0, 0xC0, 0xC0, 0xC0, 0xC0,
    0x1F, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x1B, 0x1F, 0x1C, 0x00, 0x00, 0x18, 0x18, 0x0C, 0x0F, 0x03, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0x30, 0x18, 0x18, 0x18, 0x18, 0x30, 0xF0, 0xC0,
    0x03, 0x0F, 0x0C, 0x18, 0x18, 0x18, 0x1B, 0x1F, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x0C, 0x0F, 0x03, 0xC0, 0xF0, 0x30, 0x18, 0x18, 0x00, 0xC0, 0xF0, 0x30, 0x18, 0x18, 0x18, 0x18, 0x30, 0xF0, 0xC0,
    0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0xF8, 0xF8, 0x30, 0x30, 0x60, 0x60, 0x60, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00,
    0x03, 0x0F, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x07, 0x07, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x0F, 0x03, 0xC0, 0xF0, 0x30, 0x18, 0x18, 0x18, 0x30, 0xE0, 0xE0, 0x30, 0x18, 0x18, 0x18, 0x30, 0xF0, 0xC0,
    0x03, 0x0F, 0x0C, 0x18, 0x18, 0x18, 0x18, 0x1C, 0x0F, 0x03, 0x00, 0x18, 0x18, 0x0C, 0x0F, 0x03, 0xC0, 0xF0, 0x30, 0x18, 0x18, 0x18, 0x18, 0x38, 0xF8, 0xD8, 0x18, 0x18, 0x18, 0x30, 0xF0, 0xC0,

    // Degree symbol
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // Negative symbol
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // . symbol
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38,

    // FARENHEIT
    0x1F, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // CELSIUS
    0x03, 0x0F, 0x0C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x0C, 0x0F, 0x03, 0xC0, 0xF0, 0x30, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 0xF0, 0xC0,

    // DEGREE SMALL
    0x04, 0x0A, 0x0A, 0x04, 0x00, 0x00, 0x00, 0x00};

const char logo_udgs[] = {
    // logo data
    0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x19, 0x1b, 0x19, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x18, 0x18, 0x1c, 0x0c, 0x0f, 0x07, 0x01, 0x00, 0x06, 0x06, 0x0e, 0x0c, 0x3c, 0xf8, 0xe0, 0x00,
    0x03, 0x03, 0x03, 0x43, 0xe7, 0x7e, 0x3c, 0x00, 0x19, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x08, 0x0c, 0x0e, 0x0f, 0x0f, 0x0d, 0x8c, 0x0c, 0x01, 0x01, 0x01, 0x01, 0x81, 0xc1, 0xe1, 0x71,
    0x8f, 0x8f, 0x8c, 0x8c, 0x8c, 0x8c, 0x8f, 0x8f, 0xf3, 0xf3, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xff, 0xff, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xcc, 0x0c, 0x00,
    0x39, 0x1d, 0x0f, 0x07, 0x03, 0x01, 0x00, 0x00, 0x8c, 0x8c, 0x8c, 0x8c, 0x8c, 0x8f, 0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00,
    0x04, 0x04, 0x04, 0x0e, 0x1f, 0x3f, 0xff, 0x3f, 0x7f, 0x3e, 0x1c, 0x1c, 0x22, 0xc1, 0xc1, 0xc1, 0x10, 0x10, 0x10, 0x38, 0x7c, 0xfe, 0xff, 0xfe, 0x1f, 0x0e, 0x04, 0x04, 0x04, 0x1f, 0x04, 0x04,
    0x22, 0x1c, 0x08, 0x08, 0x08, 0xff, 0x08, 0x08, 0x7c, 0x38, 0x38, 0x7c, 0xfe, 0xff, 0xfe, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x1f, 0x00, 0x08, 0x08, 0x08, 0x1c, 0x3e, 0x7f, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0xfe, 0x08, 0x08, 0x08, 0x1f, 0x08, 0x00, 0x00, 0x00, 0x38, 0x10, 0x10, 0xff, 0x10, 0x10, 0x10, 0x00, 0x10, 0x10, 0x10, 0xfc, 0x10, 0x10, 0x00, 0x00};

void screen_init(void) { 
    clrscr(); 
}

void screen_colors(unsigned long d, short offset, unsigned char *fg, unsigned char *bg, bool *day)
{
    Timestamp ts;

    timestamp(d + offset, &ts);

    switch (ts.hour)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        *day = false;
        *fg = VDP_INK_WHITE;
        *bg = VDP_INK_BLACK;
        break;
    case 5:
    case 6:
    case 7:
        *day = true;
        *bg = VDP_INK_DARK_BLUE;
        *fg = VDP_INK_WHITE;
        break;
    case 8:
    case 9:
    case 10:
        *day = true;
        *bg = VDP_INK_LIGHT_BLUE;
        *fg = VDP_INK_WHITE;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
        *day = true;
        *bg = VDP_INK_CYAN;
        *fg = VDP_INK_BLACK;
        break;
    case 17:
    case 18:
    case 19:
        *day = false;
        *bg = VDP_INK_DARK_BLUE;
        *fg = VDP_INK_WHITE;
        break;
    case 20:
    case 21:
    case 22:
    case 23:
        *day = false;
        *bg = VDP_INK_BLACK;
        *fg = VDP_INK_WHITE;
        break;
    default:
        break;
    }
}

void screen_bigprint_offsets(char *c, unsigned char *c0, unsigned char *c1, unsigned char *c2, unsigned char *c3)
{
    // 0 2 1 3
    unsigned char o = 0x80; // Initial char offset
    char t = *c;

    switch (t)
    {
    case '*':
        o += 0x28;
        break;
    case '-':
        o += 0x2C;
        break;
    case '.':
        o += 0x30;
        break;
    case 'F':
        o += 0x34;
        break;
    case 'C':
        o += 0x38;
        break;
    default:
        t -= 0x30;
        t *= 4;
        t += 0x80;
        o = t;
        break;
    }

    *c0 = o;
    *c1 = o + 2;
    *c2 = o + 1;
    *c3 = o + 3;
}

void screen_bigprint(unsigned char x, unsigned char y, char *c)
{
    x <<= 2;
    y <<= 2;

    while (*c != 0x00)
    {
        unsigned char c0, c1, c2, c3;

        screen_bigprint_offsets(c, &c0, &c1, &c2, &c3);
        if ((*c == '.') || (*c == '*'))
        {
            gotoxy(x, y);
            cprintf("%c", c1);
            gotoxy(x, y + 1);
            cprintf("%c", c3);
            x += 1;
        }
        else
        {
            gotoxy(x, y);
            cprintf("%c%c", c0, c1);
            gotoxy(x, y + 1);
            cprintf("%c%c", c2, c3);
            x += 2;
        }
        c++;
    }
}


/* ***************************************************************************** */
/* ***************************************************************************** */
/* ****************************      LOCATION          ************************* */
/* ***************************************************************************** */
/* ***************************************************************************** */

/* 
screen_location
- request the user supply the latitude and logitude of where they
  are located
*/

void screen_clear_lines(int y, int num)
{
    int i;
    for(i=0; i<num; i++)
    {
        gotoxy(0,y+i);
        cprintf("                                ");
    }
    gotoxy(0,y);
}

bool is_number(char *string, bool floating)
{
    int i;
    bool decimal = false;
    bool numbers_started = false;

    if (strlen(string) == 0)
        return false;

    for (i=0; i<strlen(string); i++)
    {
        if ((string[i] == '-') || (string[i] == '+'))
            if (numbers_started)
                return false;
            else
                continue;

        if ((string[i] == ' ') && numbers_started)
            return false;
        else
            continue;

        if (floating)
            if (string[i] == '.')
            {
                if (decimal)
                    return false;
                else
                {
                    decimal = true;
                    continue;
                }
            }

        if (! isdigit(string[i]))
            return false;
        else
            numbers_started = true;

    }
    return true;
}

bool screen_location(Location *l, bool *autoip, bool *manual)
{
    unsigned char fg = VDP_INK_BLACK;
    unsigned char bg = VDP_INK_CYAN;
    void *param = &udgs;
    int y = 10;
    int i;
    long number;
  

    console_ioctl(IOCTL_GENCON_SET_UDGS, &param);
    vdp_color(fg, bg, bg);
    clrscr();

    clear_all_sprites();

    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    vdp_color(fg, bg, bg);

    cprintf("    *** LOCATION SETTINGS ***   \n\n");
    cprintf("Automatic will use your IP to\n");
    cprintf("try and determine your location\n");
    cprintf("With Manual, you will supply the\n");
    cprintf("latitude, longitude & city info\n");


    while (1)
    {

        screen_clear_lines(y,2);

        cprintf("Do you want to use\n[A]utomatic location detection\nor\n[M]anual settings?\n(a/m) <enter to leave unchanged>\n");
        smartkeys_status("Automatic will use your computers\nIP address to determine location.");
        vdp_color(fg, bg, bg);
        gets(response);
        response[strlen(response) - 1] = '\0';
        if (strlen(response) == 0)
            return false;

        if ( (stricmp(response, "a") == 0) )
        {
            *autoip = true;
            *manual = false;
            return true;
        }

        if ( (stricmp(response, "m") == 0) )
        {
            break;
        }
    }

    console_ioctl(IOCTL_GENCON_SET_UDGS, &param);
    vdp_color(fg, bg, bg);
    clrscr();

    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    vdp_color(fg, bg, bg);

    cprintf("*** MANUAL LOCATION SETTINGS ***\n\n");
    cprintf("   Get latitude and longitude   \n");
    cprintf("  of a city from this website:  \n\n");
    cprintf("         www.latlong.net        \n\n");


    *autoip = false;
    *manual = true;
    while(1)
    {
        screen_clear_lines(y,2);

        cprintf("What is the latitude?\n<enter for %s>\n", l->latitude);
        smartkeys_status("What is the latitude?");
        vdp_color(fg, bg, bg);
        gets(response);

        // remove newline
        response[strlen(response) - 1] = '\0';
        if (strcmp(response, "") == 0)
        {
            strcpy(response, l->latitude);
            break;
        }

        if (is_number(response, true))
        {
            number = atol(number);
            if (number > 180)
                continue;

            if (number < -180)
                continue;

            break;
        }
    }

    strncpy2(l->latitude, response, sizeof(l->latitude));

    while (1)
    {
        screen_clear_lines(y,4);

        cprintf("What is the longitude?\n<enter for %s>\n", l->longitude);
        smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
        smartkeys_status("What is the longitude?");
        vdp_color(fg, bg, bg);
        gets(response);

        // remove newline
        response[strlen(response) - 1] = '\0';
        if (strcmp(response, "") == 0)
        {
            strcpy(response, l->longitude);
            break;
        }

        if (is_number(response, true))
        {
            number = atol(number);
            if (number > 180)
                continue;

            if (number < -180)
                continue;

            break;
        }
    }

    strncpy2(l->longitude, response, sizeof(l->longitude));

    screen_clear_lines(y,4);
    cprintf("What is the city name?\n<enter for %s>\n", l->city);
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("What is the city name?");
    vdp_color(fg, bg, bg);
    gets(response);

    // remove newline
    response[strlen(response) - 1] = '\0';
    if (strcmp(response, "") != 0)
        strncpy2(l->city, response, sizeof(l->city));

    screen_clear_lines(y,4);
    cprintf("What is the 2 letter\nstate/province/region code?\n<enter for %s>\n", l->region_code);
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("What is the 2 letter state/province/region code?");
    vdp_color(fg, bg, bg);
    gets(response);

    // remove newline
    response[strlen(response) - 1] = '\0';
    if (strcmp(response, "") != 0)
        strncpy2(l->region_code, response, sizeof(l->region_code));

    screen_clear_lines(y,4);
    cprintf("What is the 2 letter\ncountry code?<enter for %s>\n",l->country_code);
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("What is the 2 letter country code?");
    vdp_color(fg, bg, bg);
    gets(response);

    // remove newline
    response[strlen(response) - 1] = '\0';
    if (strcmp(response, "") != 0)
        strncpy2(l->country_code, response, sizeof(l->country_code));

    return true;
}

bool screen_options(OptionsData *o)
{
    unsigned char fg = VDP_INK_BLACK;
    unsigned char bg = VDP_INK_CYAN;
    void *param = &udgs;
    int y = 10;
    long value;

    
    console_ioctl(IOCTL_GENCON_SET_UDGS, &param);
    vdp_color(fg, bg, bg);
    clrscr();

    clear_all_sprites();

    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    vdp_color(fg, bg, bg);

    cprintf("*** PERFORMANCE SETTINGS *** \n\n");

    while(1)
    {
        screen_clear_lines(y,4);
        cprintf("Frequency of weather/forecast\nupdates in minutes?\n(<enter for %ld>\n", o->refreshIntervalMinutes);
        smartkeys_status("Frequency of weather/forecast updates in minutes?\nBetween 10 and 1380 minutes");
        vdp_color(fg, bg, bg);
        gets(response);

        // remove newline
        response[strlen(response) - 1] = '\0';

        if (strlen(response) == 0)
            return true;

        if (! is_number, false)
        {
            continue;
        } else
        {
            value = atol(response);
            if (value < 10)
                continue;
            if (value > 1380)
                continue;

            break;
        }

    }

    value = atol(response);
    o->refreshIntervalMinutes = value;

    return true;
}


/* ***************************************************************************** */
/* ***************************************************************************** */
/* ****************************         DAILY          ************************* */
/* ***************************************************************************** */
/* ***************************************************************************** */

void screen_daily(char *date, unsigned char icon, char *temperature, char *pressure, char *description, char *location, char *wind, char *feels, char *dew, char *visibility, char *timezone,
                  char *sunrise, char *sunset, char *humidity, char *clouds, char *time, unsigned char foregroundColor, unsigned char backgroundColor, bool day)
{
    void *param = &udgs;
    int x_start;

    console_ioctl(IOCTL_GENCON_SET_UDGS, &param);
    vdp_color(foregroundColor, backgroundColor, backgroundColor);
    clrscr();

    clear_all_sprites();

    if (optData.units == IMPERIAL)
        strcpy(tmp, "  SHOW\n CELSIUS");
    else
        strcpy(tmp, "  SHOW\nFARENHEIT");

    smartkeys_display("  OPTIONS", NULL, "LOCATION", " SHOW\nFORECAST", tmp, " REFRESH");

    vdp_color(foregroundColor, backgroundColor, backgroundColor);

    gotoxy(8, 0);
    cprintf("%s", date);
    clear_all_sprites();
    save_sprite(24, 24, icon, day);
    display_sprites();
    screen_bigprint(2, 1, temperature);
    gotoxy(23, 4);
    cprintf("%s", pressure);
    x_start = 36/2 - strlen(description)/2;
    if (x_start < 0)
        x_start = 0;
    gotoxy(x_start, 7);
    cprintf("%s", description);

    gotoxy(16 - strlen(location)/2, 9);
    cprintf("%s", location);

    sprintf(tmp, "WIND: %s\nFEELS LIKE: %s\n\nDEW POINT: %s\nVISIBILITY: %s\n\nTIME ZONE: %s", wind, feels, dew, visibility, timezone);
    smartkeys_puts(0, 96, tmp);

    sprintf(tmp, "SUNRISE: %s\nSUNSET: %s\n\nHUMIDITY: %s\nCLOUDS: %s\n\nTIME: %s", sunrise, sunset, humidity, clouds, time);
    smartkeys_puts(160, 96, tmp);
}

/* ***************************************************************************** */
/* ***************************************************************************** */
/* ****************************       WELCOME          ************************* */
/* ***************************************************************************** */
/* ***************************************************************************** */

void screen_welcome(void)
{
    void *param = &logo_udgs;

    // Use custom font
    console_ioctl(IOCTL_GENCON_SET_UDGS, &param);
    smartkeys_set_mode();

             // Fujinet Logo
    printf("\x20\x20\x20\x20\x20\x9A\x9B\x9C\x20\x20\x20\x20\x20       OPEN WEATHER");
    printf("\x80\x81\x82\x83\x84\x94\x95\x96\x8A\x8B\x8C\x8D\x8e       CLIENT " CLIENT_VERSION "\n");
    printf("\x85\x86\x87\x88\x89\x97\x98\x99\x8F\x90\x91\x92\x93           for\n");
    printf("\x20\x20\x20\x20\x20\x20\x9D\x9E\x9F\x20\x20\x20\x20       COLECO  ADAM\n");
}

/* ***************************************************************************** */
/* ***************************************************************************** */
/* ****************************       OPTIONS          ************************* */
/* ***************************************************************************** */
/* ***************************************************************************** */


void screen_options_init(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\n  INITIALIZING OPTIONS...");
    csleep(STATUS_DELAY);
}

void screen_options_saving(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\n   SAVING OPTIONS...");
    csleep(STATUS_DELAY);
}

void screen_options_success(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\nSUCCESS LOADING OPTIONS...");
    csleep(STATUS_DELAY);
}

void screen_options_load_failed(void)
{
    snprintf(response, sizeof(response),  "Version '%s' loaded, expected '%s'\n  USING DEFAULT OPTIONS.", 
                            optData.version,
                            OPTIONS_VERSION);

    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status(response);
    csleep(STATUS_DELAY);
}

void screen_options_could_not_save(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\n  COULD NOT SAVE OPTIONS.");
    csleep(STATUS_DELAY);
}

void screen_location_ip_detect(void)
{
    clrscr();
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\nDETECTING YOUR LOCATION VIA IP...");
    csleep(STATUS_DELAY);
}


void screen_location_position_detect(void)
{
    clrscr();
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\nUSING MANUAL LOCATION...");
    csleep(STATUS_DELAY);
}

void screen_location_position_found(void)
{
    clrscr();
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\nMANUAL LOCATION FOUND.");
    csleep(STATUS_DELAY);
}


void screen_location_load_failed(void)
{
    snprintf(response, sizeof(response),  "Version '%s' loaded, expected '%s'\n  USING DEFAULT LOCATION.", 
                            locData.version,
                            LOCATION_VERSION);

    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status(response);
    csleep(STATUS_DELAY);
}

void screen_location_load_defaults(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\nUSING DEFAULT LOCATION...");
    csleep(STATUS_DELAY);
}

void screen_location_success(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\nSUCCESS DETECTING LOCATION...");
    csleep(STATUS_DELAY);
}

void screen_location_success_(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\nSUCCESS DETECTING LOCATION...");
    csleep(STATUS_DELAY);
}

void screen_location_could_not_ip_detect(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\n  COULD NOT DETECT YOUR IP LOCATION...");
    csleep(STATUS_DELAY);
}



void screen_location_could_not_save(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\n  COULD NOT SAVE LOCATION.");
    csleep(STATUS_DELAY);
}

void screen_location_saving(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\n  SAVING LOCATION...");
    csleep(STATUS_DELAY);
}

void screen_location_saved(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\n  LOCATION SAVED.");
    csleep(STATUS_DELAY);
}

void screen_location_could_not_detect(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\n  COULD NOT DETECT YOUR LOCATION...");
    csleep(STATUS_DELAY);  
}

void screen_current_time(void)
{
    io_time(&current_time);
    snprintf(response, sizeof(response),  "Compile Time: " __DATE__ " "__TIME__ "\nTime: %2u%02u/%02u/%02u %02u:%02u:%02u\n", 
    (unsigned int) current_time.century, (unsigned int) current_time.year,(unsigned int) current_time.month, (unsigned int) current_time.day, 
    (unsigned int) current_time.hour, (unsigned int) current_time.minute, (unsigned int) current_time.second);

    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status(response);
    csleep(STATUS_DELAY); 
}


/* ***************************************************************************** */
/* ***************************************************************************** */
/* ****************************       WEATHER          ************************* */
/* ***************************************************************************** */
/* ***************************************************************************** */

void screen_weather_init(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\n  RETRIEVING WEATHER INFORMATION...");
}

// step 4
void screen_weather_could_not_get()
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\n  COULD NOT RETRIEVE WEATHER DATA.");
    csleep(STATUS_DELAY);
}

// step 3
void screen_weather_parsing()
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\n  PARSING WEATHER DATA...");
}

/* ***************************************************************************** */
/* ***************************************************************************** */
/* ****************************       FORECAST         ************************* */
/* ***************************************************************************** */
/* ***************************************************************************** */

void screen_forecast_init(void)
{
    clrscr();

    clear_all_sprites();
}

void screen_forecast_parsing(void)
{
    smartkeys_display(NULL, NULL, NULL, NULL, NULL, NULL);
    smartkeys_status("\n  PARSING FORECAST DATA, PLEASE WAIT...");
}

void screen_forecast(unsigned char i, ForecastData *f, unsigned char foregroundColor, unsigned char backgroundColor, bool day)
{
    //                    i  D  d   l   h
    unsigned char x[5] = {0, 7, 15, 21, 27};
    unsigned char iy[5] = {15, 55, 90, 128};
    unsigned char y = (i % 4) * 5;
    void *param = &udgs;
 
    foregroundColor = VDP_INK_WHITE;
    backgroundColor = VDP_INK_BLACK;

    if (y == 0)
    {
        console_ioctl(IOCTL_GENCON_SET_UDGS, &param);
        vdp_color(foregroundColor, backgroundColor, backgroundColor);
        clrscr();

        gotoxy(x[1],y);
        vdp_color(backgroundColor, foregroundColor, foregroundColor);
        cprintf("%25s", " ");

        gotoxy(x[1],y);
        cprintf("  DATE");

        gotoxy(x[2],y);
        cprintf("DAY");

        gotoxy(x[3],y);
        cprintf("LOW");

        gotoxy(x[4],y);
        cprintf("HIGH");    
    }

    vdp_color(foregroundColor, backgroundColor, backgroundColor);
    y += 2;

    // icon
    save_sprite(4, iy[i], f->icon, day);
    display_sprites();

    // date
    gotoxy(x[1],y);
    cprintf("%5s", f->date);

    // day of week
    gotoxy(x[2],y);
    cprintf("%-5s", f->dow);

    // Low
    gotoxy(x[3],y);
    cprintf("%5s", f->lo);

    // High
    gotoxy(x[4],y);
    cprintf("%5s", f->hi);

    y++;
    gotoxy(x[1],y);
    cprintf("%25s", f->desc);

}

void screen_forecast_keys(void)
{
    char tmp2[20];

    if (optData.units == IMPERIAL)
        strcpy(tmp, "  SHOW\n CELSIUS");
    else
        strcpy(tmp, "  SHOW\nFARENHEIT");

    if (forecast_offset == 0)
    {
        strcpy(tmp2, "  NEXT\n  PAGE");
    } else
    {
        strcpy(tmp2, "  PREV\n  PAGE");
    }
        
    smartkeys_display("  OPTIONS", tmp2, "LOCATION", "  SHOW\n DAILY", tmp, " REFRESH");
}

void screen_weather_keys(void)
{

    if (optData.units == IMPERIAL)
        strcpy(tmp, "  SHOW\n CELSIUS");
    else
        strcpy(tmp, "  SHOW\nFARENHEIT");

        
    smartkeys_display("  OPTIONS", NULL, "LOCATION", "  SHOW\nFORECAST", tmp, " REFRESH");
}

