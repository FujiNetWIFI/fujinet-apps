/**
 * @brief petsciiterm host directory
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details
 */

#include <c64.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "color.h"

#define ENTRY_Y 3
#define FILL_CHAR 0xA4
#define ENTRY_MAX 26
#define ENTRY_URL_Y ENTRY_MAX + 1

#define COLUMN_1 2
#define COLUMN_2 20

static unsigned char selected_entry = 0;
static bool directory_running;

extern void ihdone();  // srq.s

char dial_url[40];

/**
 * @brief a Directory entry
 */
static struct _directory
{
    char name[20];
    char url[40];
} d[ENTRY_MAX] =
{
    {"BBS RetroCampus", "TELNET://BBS.RETROCAMPUS.COM:6510/"},
    {"Boar's Head Tavern", "TELNET://BYOB.HOPTO.ORG:64128"},
    {"13TH FLOOR BB3", "TELNET://13TH.HOYVISION.COM:6400"},
    {"C64 Wordle", "TELNET://WORDLE64.MYWIRE.ORG:8877"}
};

/**
 * @brief show entry detail (URL)
 * @param n The entry to show
 */
void entry_url (unsigned char n)
{
    unsigned char y;
    unsigned char i;
    y = (ENTRY_MAX >> 1) + 4;

    // Box
    textcolor (COLOR_GRAY2);
    revers (1);
    gotoxy (0, y);
    for (i = 0; i < 160; i++)
        cputc (' ');
    revers (0);
    textcolor (COLOR_GRAY1);
    for (i = 0; i < 40; i++)
        cputc (0xB7);

    // header and entry

    textcolor (COLOR_GRAY2);
    revers (1);
    gotoxy (0, y + 1);
    cprintf ("%-40s", "URL:");
    textcolor (COLOR_CYAN);
    cprintf ("%-40s", d[n].url);
}

/**
 * @brief show directory entry
 * @param n The entry # to show
 * @param s is selected? (true/false)
 */
void entry (unsigned char n, bool s)
{
    unsigned char x, y;
    char t[19];

    memset (&t[0], FILL_CHAR, sizeof (t));
    t[18] = 0x00;
    memcpy (&t, d[n].name, strlen (d[n].name));

    y = ENTRY_Y + (n >> 1);
    x = n % 2 ? COLUMN_2 : COLUMN_1;

    revers (true);
    textcolor (s ? COLOR_CYAN : COLOR_LIGHTBLUE);
    gotoxy (x, y);
    cprintf (" %s ", t);
    revers (false);

    if (s)
    {
        entry_url (n);
    }
}

/**
 * @brief display all entries; highlight selected
 */
void entries (void)
{
    unsigned char i;

    for (i = 0; i < ENTRY_MAX; i++)
        if (i == selected_entry)
            entry (i, true);
        else
            entry (i, false);
}

/**
 * @brief show key legend at bottom
 */
void keys (void)
{
    textcolor (COLOR_WHITE);
    gotoxy (0, 22);

    cputs ("\xB6");
    revers (1);
    cputs ("CRSR");
    revers (0);
    cputs ("\xB5");
    cputs ("SELECT");

    cputs ("\xB6");
    revers (1);
    cputs ("E");
    revers (0);
    cputs ("\xB5");
    cputs ("EDIT");

    cputs ("\xB6");
    revers (1);
    cputs ("O");
    revers (0);
    cputs ("\xB5");
    cputs ("OTHER");

    cputs ("\xB6");
    revers (1);
    cputs ("Q");
    revers (0);
    cputs ("\xB5");
    cputs ("QUIT\r\n");

    textcolor (COLOR_GRAY1);
    cprintf ("%40s", "powered by #FUJINET!");
}

/**
 * @brief called when CRSR UP pressed
 */
static void up (void)
{
    unsigned char x;

    x = selected_entry % 2;

    if (selected_entry < 2)
        selected_entry = ENTRY_MAX - x;
    else
        selected_entry -= 2;
}

/**
 * @brief called when CRSR DN pressed
 */
static void down (void)
{
    unsigned char x;

    x = selected_entry % 2;

    if (selected_entry == (ENTRY_MAX - x))
        selected_entry = 0 + x;
    else
        selected_entry += 2;
}

/**
 * @brief called when CRSR LF or RT pressed
 */
static void leftright (void)
{
    unsigned char x;

    x = selected_entry % 2;

    if (x)
        selected_entry--;
    else
        selected_entry++;
}

/**
 * @brief interactively edit string s
 * @param x horizontal position (0-39)
 * @param y vertical position (0-24)
 * @param l Length of field (0-255)
 * @param s char array var to modify
 */
static void input (unsigned char x,
                   unsigned char y,
                   unsigned char l,
                   char *s)
{
    unsigned char o;
    unsigned char i;

    textcolor (COLOR_CYAN);

    cursor (true);

    gotoxy (x, y);

    for (i = 0; i < l; i++)
        cputc (' ');

    gotoxy (x, y);
    cputs (s);

    o = strlen (s);

    while (1)
    {
        char c;

        c = cgetc();

        if (c == 0x0D) // RETURN
            break;

        if (c == 0x14) // DEL
        {
            if (!o) // at beginning.
                continue;

            cputcxy (x + o, y, ' ');
            s[o--] = '\0';
            gotoxy (x + o, y);
        }

        if (c > 0x1F)
        {
            if (o == l - 1) // at end of field, ignore.
                continue;

            s[o++] = c;
            putchar (c);
        }

    }

    cursor (false);
}

/**
 * @brief Edit d[selected_entry]
 */
static void edit (void)
{
    unsigned char x;
    unsigned char y, z;

    x = ((selected_entry % 2) ? COLUMN_2 : COLUMN_1) + 1;
    y = ENTRY_Y + (selected_entry >> 1);
    z = (ENTRY_MAX >> 1) + 5; // Header and detail input Y

    revers (1);
    textcolor (COLOR_GRAY2);
    gotoxy (0, z);
    cprintf ("%-40s", "ENTER NEW TITLE ^^");
    input (x, y, strlen (d[selected_entry].name), d[selected_entry].name);

    textcolor (COLOR_GRAY2);
    gotoxy (0, z);
    cprintf ("%-40s", "ENTER NEW URL:");
    input (0, z + 1, strlen (d[selected_entry].url), d[selected_entry].url);
}

/**
 * @brief Dial selected entry
 * @verbose (actually, pass it back to term())
 */
static void dial (void)
{
    // Display message if selected URL is empty.
    if (!strlen (d[selected_entry].url))
    {
        unsigned char y;
        unsigned short i;

        textcolor (COLOR_GRAY2);
        y = (ENTRY_MAX >> 1) + 5;
        gotoxy (0, y);
        revers (1);
        cprintf ("%-80s", "DIRECTORY ENTRY EMPTY.");
        revers (0);

        for (i = 0; i < 4096; i++); // message delay.
        return;
    }

    strcpy (dial_url, d[selected_entry].url);
    directory_running = false;
}

/**
 * @brief Input, then Dial entered entry
 */
static void other (void)
{
    char url[40];
    char y;

    y = (ENTRY_MAX >> 1) + 5;

    memset (url, 0, sizeof (url) - 1);

    textcolor (COLOR_GRAY2);
    gotoxy (0, y);
    cprintf ("DIAL OTHER URL:\n");
    input (0, y + 1, sizeof (url), url);

    if (strlen (url))
    {
        strcpy (dial_url, url);
        directory_running = false;
    }
}

/**
 * @brief Quit petsciiTERM
 */
static void quit (void)
{
    ihdone();
    revers (0);
    bordercolor (COLOR_LIGHTBLUE);
    textcolor (COLOR_LIGHTBLUE);
    bgcolor (COLOR_BLUE);
    clrscr();
    printf ("Thank you, for using PETSCIIterm.\n");
    exit (0);
}

/**
 * @brief go to dialing directory
 */
void directory (void)
{
    // Set up screen
    bordercolor (DIRECTORY_BORDER_COLOR);
    bgcolor (DIRECTORY_BACKGROUND_COLOR);
    clrscr();

    keys();

    textcolor (COLOR_GREEN);
    chlinexy (0, 1, 40);

    textcolor (COLOR_LIGHTBLUE);
    revers (1);
    cputsxy (8, 1, " PETSCIIterm DIALING DIRECTORY ");

    revers (0);
    textcolor (COLOR_BLUE);
    cputsxy (8, 2, "\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7");

    textcolor (COLOR_BLUE);
    cputsxy (0, (ENTRY_MAX >> 1) + ENTRY_Y, "\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7\xB7");

    entries();

    directory_running = true;

    while (directory_running)
    {
        switch (cgetc())
        {
            case 0x0d: // RETURN
                dial();
                break;
            case 0x91: // UP
                up();
                break;
            case 0x11: // DOWN
                down();
                break;
            case 0x1D: // RIGHT
            case 0x9D: // LEFT
                leftright();
                break;
            case 'e':
            case 'E':  // EDIT
                edit();
                break;
            case 'o':
            case 'O':  // OTHER
                other();
                break;
            case 'q':
            case 'Q':  // QUIT
                quit();
                break;
        }

        entries();
    }
}
