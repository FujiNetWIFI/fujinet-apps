/**
 * @brief FujiNet VT100 terminal for the CoCo 3.
 *
 * Mirrors the Atari port (prompt for a devicespec, open N:, loop) but uses
 * the Apple II port's polling read loop since the CoCo lacks PROCEED.
 * Transport is fujinet-lib-coco.
 */

#include <cmoc.h>
#include <coco.h>
#include "vt100.h"
#include "screen.h"
#include "fujinet-network.h"
#include "fujinet-fuji.h"

#define RXSZ 320

#ifndef VT100_VERSION
#define VT100_VERSION "dev"
#endif

#define PB_CREATOR  0x0901
#define PB_APP      0x02
#define PB_SLOTS    8
#define PB_SETTINGS 8           /* appkey key just past the 8 phonebook slots */
#define PB_NAME_LEN 16
#define PB_URL_LEN  47

#define SCH_TELNET  0
#define SCH_SSH     1
#define SCH_CPM     2
#define SCH_OTHER   3

typedef struct
{
    char name[PB_NAME_LEN];
    char url[PB_URL_LEN];
    unsigned char scheme;       /* trailing byte: old 63-byte entries load as telnet */
} PBEntry;

extern void (*term_sendback)(char c);   /* set so DSR/cursor replies go to the wire */
extern void (*term_bell)(void);          /* set so a received BEL rings the speaker */

static char devicespec[160];
static unsigned char rx_buf[RXSZ];
static unsigned char running;

/* Bare URL of a one-shot N-path session, held for offer_save(); empty if
   none pending. oneshot_too_long: URL exceeded PB_URL_LEN, show a message
   instead of the save prompt. */
static char oneshot_url[PB_URL_LEN];
static unsigned char oneshot_too_long;
static unsigned char oneshot_scheme;

/* Phonebook scratch carved from rx_buf (idle at the prompt). PB_KEYBUF needs
   66 bytes - fuji_read_appkey wants keysize+2. */
#define PB_KEYBUF  ((uint8_t *)(rx_buf + 0))    /*   0..65  */
#define PB_LINE    ((char *)   (rx_buf + 80))   /*  80..159 */
#define PB_NAMEBUF ((char *)   (rx_buf + 160))  /* 160..176 */
#define PB_URLBUF  ((char *)   (rx_buf + 180))  /* 180..227 */
#define PB_CURRENT ((PBEntry *)(rx_buf + 256))  /* 256..319 */

/* ---- terminal stream output ---- */

/* Hot path: when the decoder is in CHAR state, bulk-write runs of printable
   bytes to the shadow in one screen_puts_run call; route control/escape
   bytes through the engine. */
static void vt_write(const unsigned char *buf, uint16_t len)
{
    uint16_t j = 0, s;
    unsigned char c;

    while (j < len)
    {
        c = buf[j];
        if (c >= 0x20 && c < 0x7F && vt100_char_state())
        {
            s = j;
            do { j++; } while (j < len && buf[j] >= 0x20 && buf[j] < 0x7F);
            screen_puts_run(&buf[s], j - s);
        }
        else
        {
            vt100((char) c);
            j++;
        }
    }
}

static void feed(const char *s)
{
    vt_write((const unsigned char *) s, (uint16_t) strlen(s));
}

/* DSR / cursor-position replies from the engine -> network */
static void net_sendback(char c)
{
    network_write(devicespec, (const unsigned char *) &c, 1);
}

/* Ring the speaker for a received BEL. ROM sound() drives the 6-bit DAC at
   $FF20 and the sound-enable at $FF23. A bit-banger FujiNet (fujinet-coco-
   devkitc) shares $FF20 for serial TX, so save/restore both registers to leave
   the serial idle line untouched; a DriveWire/Becker FujiNet (fujiversal)
   doesn't use $FF20 for comms, so the save/restore is simply harmless there. */
static void net_bell(void)
{
    unsigned char *pia20 = (unsigned char *) 0xFF20;
    unsigned char *pia23 = (unsigned char *) 0xFF23;
    unsigned char s20 = *pia20;
    unsigned char s23 = *pia23;

    sound(150, 2);

    *pia23 = s23;
    *pia20 = s20;
}

/* ---- prompt input ---- */

/* ALT + the number row produces the 10 ASCII characters the CoCo keyboard
   lacks. Indexed by (key - '0'):  0='^' 1='[' 2=']' 3='{' 4='}' 5='|' 6='\'
   7='_' 8='~' 9='`'  */
static const char alt_syms[10] = { '^','[',']','{','}','|','\\','_','~','`' };

/* Apply the keyboard scheme by probing modifiers: ALT+digit -> missing symbol,
   CTRL+letter -> control code, BREAK -> ESC, default upper->lower fold (SHIFT
   keeps caps). Arrow keys are handled by the caller (out_keys). Stray modifier
   keystrokes (ALT alone, CTRL alone) are swallowed. */
static unsigned char decode_key(unsigned char k)
{
    if (!k)
        return 0;

    if (k == 0x03)                                   /* BREAK -> ESC */
        return 0x1B;

    if (isKeyPressed(KEY_PROBE_ALT, KEY_BIT_ALT))
    {
        if (k >= '0' && k <= '9')
            return alt_syms[k - '0'];
        return 0;
    }

    if (isKeyPressed(KEY_PROBE_CTRL, KEY_BIT_CTRL))
    {
        if (k >= 'A' && k <= 'Z') return k & 0x1F;
        if (k >= 'a' && k <= 'z') return k & 0x1F;
        return 0;
    }

    if (k >= 'A' && k <= 'Z' && !isKeyPressed(KEY_PROBE_SHIFT, KEY_BIT_SHIFT))
        k += 0x20;                                   /* default to lower-case */

    return k;
}

static unsigned char read_key(void)
{
    return decode_key(inkey());
}

/* 80-col line editor, blinking block cursor. Returns the line in buf (no \n). */
static void term_get_line(char *buf, unsigned char max)
{
    unsigned char i = 0, k, blink = 1;
    word last = getTimer();

    for (;;)
    {
        word now = getTimer();
        if ((unsigned) (now - last) >= 12)        /* ~0.2s blink */
        {
            last = now;
            blink ^= 1;
            screen_show_cursor(blink);
            screen_flush();
        }

        k = read_key();
        if (!k)
            continue;

        screen_show_cursor(1);                    /* solid while typing */

        if (k == 13)
            break;
        if (k == 8)                               /* BS (left arrow) */
        {
            if (i > 0)
            {
                i--;
                /* BS only moves: erase by overwriting with a space and stepping back */
                vt100(8);
                screen_putc(' ');
                vt100(8);
            }
        }
        else if (k >= 0x20 && k < 0x7F && i < (unsigned char) (max - 1))
        {
            buf[i++] = k;
            vt100((char) k);
        }
        screen_flush();
    }

    buf[i] = 0;
    screen_show_cursor(1);
    feed("\r\n");
    screen_flush();
}

/* Prompt R/C, apply the palette, and persist the choice (appkey PB_SETTINGS).
   Used on first run and from the phonebook menu's "M" option. */
static void monitor_prompt(void)
{
    unsigned char composite = 0;
    unsigned char k;

    feed("\x1b[2J\x1b[HMONITOR TYPE?\r\n\r\n");
    feed("  R = RGB (DEFAULT)\r\n");
    feed("  C = COMPOSITE\r\n");
    screen_flush();

    while ((k = inkey()) == 0) ;
    if (k == 'C' || k == 'c')
        composite = 1;

    screen_palette(composite);
    fuji_write_appkey(PB_SETTINGS, 1, &composite);
}

/* Monitor type persists in the phonebook's appkey store (key PB_SETTINGS).
   Prompt only on first run; later runs read the saved choice and skip it. */
static void choose_monitor(void)
{
    uint16_t count = 0;

    if (fuji_read_appkey(PB_SETTINGS, &count, PB_KEYBUF) && count)
    {
        if (PB_KEYBUF[0])
            screen_palette(1);
        return;
    }

    monitor_prompt();
}

/* ---- URL prompt and credential finalization ---- */

static unsigned char starts_with_ci(const char *u, const char *p)
{
    for (; *p; u++, p++)
    {
        char a = (*u >= 'A' && *u <= 'Z') ? (char) (*u + 0x20) : *u;
        if (a != *p)
            return 0;
    }
    return 1;
}

/* By scheme prefix; bare URL = telnet, an unrecognized "scheme://" = SCH_OTHER. */
static unsigned char url_scheme(const char *u)
{
    if (starts_with_ci(u, "telnet:")) return SCH_TELNET;
    if (starts_with_ci(u, "ssh:"))    return SCH_SSH;
    if (starts_with_ci(u, "cpm:"))    return SCH_CPM;
    if (strstr(u, "://"))             return SCH_OTHER;
    return SCH_TELNET;
}

/* Build "N:[scheme://]url"; CP/M is always N:cpm:/// regardless of url. */
static void set_devicespec_from_url(const char *url, unsigned char scheme)
{
    const char *body = url;

    if (scheme == SCH_CPM)
    {
        strcpy(devicespec, "N:cpm:///");
        return;
    }

    if ((body[0] == 'N' || body[0] == 'n') && body[1] == ':')
        body += 2;

    strcpy(devicespec, "N:");
    if (!strstr(body, "://"))
        strcat(devicespec, scheme == SCH_CPM ? "cpm://" :
                           scheme == SCH_SSH ? "ssh://" : "telnet://");
    strcat(devicespec, body);
}

/* True if the authority already carries credentials (an '@' between "://" and
   the next '/' or '?'), e.g. ssh://user:pass@host - then skip the cred prompt. */
static unsigned char url_has_creds(const char *url)
{
    const char *p = strstr(url, "://");
    if (!p)
        return 0;
    for (p += 3; *p && *p != '/' && *p != '?'; p++)
        if (*p == '@')
            return 1;
    return 0;
}

/* Prompt for user/pass, splice into devicespec as user:pass@host, append the
   ?term/?cols/?rows the firmware reads for telnet TTYPE/NAWS and ssh pty-req.
   Skips the prompt entirely when the URL already embeds credentials. */
static void prompt_creds_and_finalize(void)
{
    char *user = (char *) rx_buf + 0;       /*   0..47  */
    char *pass = (char *) rx_buf + 48;      /*  48..95  */
    char *full = (char *) rx_buf + 96;      /*  96..255 */
    char *sep;
    unsigned int pre;

    /* CP/M and unknown schemes: no login prompt, no ?term. */
    if (url_scheme(devicespec + 2) >= SCH_CPM)
        return;

    if (!url_has_creds(devicespec))
    {
        feed("\r\nUSERNAME (BLANK = NONE)?\r\n");
        screen_flush();
        term_get_line(user, 48);

        if (user[0])
        {
            feed("PASSWORD (BLANK = NONE)?\r\n");
            screen_flush();
            term_get_line(pass, 48);

            sep = strstr(devicespec, "://");
            if (sep)
            {
                pre = (unsigned int) (sep - devicespec) + 3;
                memcpy(full, devicespec, pre);
                full[pre] = 0;
                strcat(full, user);
                if (pass[0])
                {
                    strcat(full, ":");
                    strcat(full, pass);
                }
                strcat(full, "@");
                strcat(full, sep + 3);
                strcpy(devicespec, full);
            }
        }
    }

    if (strlen(devicespec) + 28 < sizeof(devicespec))
    {
        strcat(devicespec, strstr(devicespec, "?") ? "&" : "?");
        strcat(devicespec, "term=vt100&cols=80&rows=24");
    }
}

/* Returns 0 if user pressed ENTER on a blank line, 1 with devicespec set. */
static unsigned char prompt_url(void)
{
    char *line = (char *) rx_buf;

    feed("\x1b[2J\x1b[HFUJINET VT100 TERMINAL\r\n\r\n");
    feed("DEVICESPEC? (BLANK = PHONEBOOK)\r\n");
    feed("E.G. HOST:PORT  (TELNET:// & N: ASSUMED)\r\n\r\n");
    feed("F1 AT MENU = FULL KEY HELP\r\n\r\n");
    screen_flush();

    term_get_line(line, 96);
    if (line[0] == 0)
        return 0;

    set_devicespec_from_url(line, SCH_TELNET);
    return 1;
}

/* ---- phonebook (appkey-backed) ---- */

/* Returns 1 if a non-empty entry was decoded. *out is zeroed first, so empty
   slots and read failures look identical to the caller. */
static unsigned char pb_load(unsigned char idx, PBEntry *out)
{
    uint16_t count = 0;
    uint8_t *buf = PB_KEYBUF;

    memset(out, 0, sizeof(PBEntry));

    if (!fuji_read_appkey(idx, &count, buf))
        return 0;
    if (count == 0)
        return 0;
    if (count > sizeof(PBEntry))
        count = sizeof(PBEntry);
    memcpy(out, buf, count);

    out->name[PB_NAME_LEN - 1] = 0;        /* enforce nul-termination */
    out->url[PB_URL_LEN - 1] = 0;

    /* URL scheme overrides a stale stored type (CP/M url "/" has no "://"). */
    if (strstr((char *) out->url, "://"))
        out->scheme = url_scheme((char *) out->url);

    return out->name[0] ? 1 : 0;
}

static unsigned char pb_save(unsigned char idx, const PBEntry *in)
{
    return fuji_write_appkey(idx, sizeof(PBEntry), (uint8_t *) in);
}

/* Shown for CP/M entries in place of the (too-long-to-store) name. */
static const char cpm_label[] = "FujiNet Internal CP/M";

/* Full repaint (8 appkey reads). Arrow keys use pb_move_selection instead. */
static void pb_draw_menu(unsigned char sel)
{
    char *line = PB_LINE;
    PBEntry *e = PB_CURRENT;
    unsigned char i, j;

    screen_overlay_clear();
    screen_overlay_line(0, "FUJINET VT100 - PHONEBOOK");
    screen_overlay_line(1, "  T/S/C = TELNET/SSH/CPM");

    for (i = 0; i < PB_SLOTS; i++)
    {
        memset(line, 0, 80);
        line[0] = (i == sel) ? '>' : ' ';
        line[1] = ' ';
        line[2] = '1' + i;
        line[3] = '.';
        line[4] = ' ';

        if (pb_load(i, e))
        {
            line[5] = e->scheme == SCH_CPM ? 'C' : e->scheme == SCH_SSH ? 'S' :
                      e->scheme == SCH_TELNET ? 'T' : ' ';
            line[6] = ' ';
            if (e->scheme == SCH_CPM)
            {
                strcpy(line + 7, cpm_label);
            }
            else
            {
                for (j = 0; j < PB_NAME_LEN - 1 && e->name[j]; j++)
                    line[7 + j] = e->name[j];
                for (; j < PB_NAME_LEN; j++)
                    line[7 + j] = ' ';
                line[7 + PB_NAME_LEN] = ' ';
                for (j = 0; j < PB_URL_LEN - 1 && e->url[j]; j++)
                    line[7 + PB_NAME_LEN + 1 + j] = e->url[j];
            }
        }
        else
        {
            strcpy(line + 5, "(EMPTY)");
        }
        screen_overlay_line(2 + i, line);
    }

    screen_overlay_line(11, "UP/DN OR 1-8 MOVE   ENTER CONNECT");
    screen_overlay_line(12, "E EDIT   D DELETE   N NEW   Q QUIT");
    screen_overlay_line(13, "M MONITOR   F1 KEY HELP");
    screen_overlay_line(15, "IN SESSION: F1 = HELP");
    screen_overlay_line(16, "  CTRL-BREAK = DISCONNECT");
}

/* screen_overlay_line stops at NUL, so a one-char string only touches col 0. */
static void pb_move_selection(unsigned char old_sel, unsigned char new_sel)
{
    screen_overlay_line(2 + old_sel, " ");
    screen_overlay_line(2 + new_sel, ">");
}

/* Blank input keeps the current field. Saves only when both end up non-empty. */
static void pb_edit(unsigned char idx)
{
    PBEntry *e = PB_CURRENT;
    char *name = PB_NAMEBUF;
    char *url = PB_URLBUF;
    char nb[4];

    unsigned char scheme;

    pb_load(idx, e);

    feed("\x1b[2J\x1b[HEDIT SLOT ");
    itoa10(idx + 1, nb);
    feed(nb);
    feed("\r\n\r\n");

    feed("CURRENT TYPE: ");
    feed(e->scheme == SCH_CPM ? "CPM" : e->scheme == SCH_SSH ? "SSH" :
         e->scheme == SCH_TELNET ? "TELNET" : "OTHER");
    feed("\r\nTYPE (T=TELNET S=SSH C=CPM, BLANK=OTHER)?\r\n");
    screen_flush();
    nb[0] = 0;
    term_get_line(nb, sizeof(nb));

    if (nb[0] == 'C' || nb[0] == 'c')      scheme = SCH_CPM;
    else if (nb[0] == 'S' || nb[0] == 's') scheme = SCH_SSH;
    else if (nb[0] == 'T' || nb[0] == 't') scheme = SCH_TELNET;
    else                                   scheme = e->scheme;

    if (scheme == SCH_CPM)
    {
        memset(e, 0, sizeof(PBEntry));
        strcpy(e->name, "CP/M");      /* placeholder; menu shows cpm_label */
        strcpy(e->url, "/");
        e->scheme = SCH_CPM;
        pb_save(idx, e);
        return;
    }

    feed("\r\nCURRENT NAME: ");
    feed(e->name[0] ? (const char *) e->name : "(NONE)");
    feed("\r\nNEW NAME (BLANK = KEEP, MAX 15)?\r\n");
    screen_flush();
    name[0] = 0;
    term_get_line(name, PB_NAME_LEN);

    feed("\r\nCURRENT URL: ");
    feed(e->url[0] ? (const char *) e->url : "(NONE)");
    feed("\r\nNEW URL (BLANK = KEEP, MAX 46)?\r\n");
    screen_flush();
    url[0] = 0;
    term_get_line(url, PB_URL_LEN);

    if (name[0])
        strcpy(e->name, name);
    if (url[0])
        strcpy(e->url, url);

    e->scheme = scheme;
    /* explicit scheme in the URL overrides the picked type */
    if (strstr(e->url, "://"))
        e->scheme = url_scheme(e->url);

    if (e->name[0] && e->url[0])
        pb_save(idx, e);
}

/* Block on inkey() + decode_key until a non-zero result. */
static unsigned char wait_key(void)
{
    unsigned char k;
    do {
        while ((k = inkey()) == 0) ;
        k = decode_key(k);
    } while (k == 0);
    return k;
}

/* Y/N -> slot 1-8 -> NAME, or a "too long" message if the URL didn't fit.
   Driven by oneshot_url / oneshot_too_long which the N handler sets. */
static void offer_save(void)
{
    PBEntry *e = PB_CURRENT;
    char *name = PB_NAMEBUF;
    unsigned char k, slot;

    if (oneshot_too_long)
    {
        feed("\x1b[2J\x1b[HURL TOO LONG TO SAVE (>46)\r\n\r\nPRESS A KEY\r\n");
        screen_flush();
        wait_key();
        oneshot_too_long = 0;
        return;
    }

    if (!oneshot_url[0])
        return;

    feed("\x1b[2J\x1b[HSAVE TO PHONEBOOK? (Y/N)\r\n\r\nURL: ");
    feed(oneshot_url);
    feed("\r\n");
    screen_flush();

    k = wait_key();
    if (k != 'y')
    {
        oneshot_url[0] = 0;
        return;
    }

    feed("\r\nSLOT 1-8?\r\n");
    screen_flush();

    k = wait_key();
    if (k < '1' || k > '8')
    {
        oneshot_url[0] = 0;
        return;
    }
    slot = k - '1';

    feed("\r\nNAME (MAX 15)?\r\n");
    screen_flush();
    name[0] = 0;
    term_get_line(name, PB_NAME_LEN);

    if (name[0])
    {
        memset(e, 0, sizeof(PBEntry));
        strcpy(e->name, name);
        strcpy(e->url, oneshot_url);
        e->scheme = oneshot_scheme;
        pb_save(slot, e);
    }

    oneshot_url[0] = 0;
}

static void show_help(void);

/* Returns 1 with devicespec set and ready to connect, 0 if the user quit. */
static unsigned char pb_menu(void)
{
    PBEntry *e = PB_CURRENT;
    unsigned char sel = 0;
    unsigned char k;
    const char *bare;

    offer_save();

    for (;;)
    {
        pb_draw_menu(sel);

        for (;;)
        {
            for (;;)
            {
                if (isKeyPressed(KEY_PROBE_F1, KEY_BIT_F1))
                {
                    show_help();
                    pb_draw_menu(sel);
                    continue;
                }
                if ((k = inkey()) != 0)
                    break;
            }
            k = decode_key(k);
            if (k == 0)
                continue;

            if (k == 0x5E)                   /* up arrow */
            {
                if (sel > 0) { pb_move_selection(sel, sel - 1); sel--; }
                continue;
            }
            if (k == 0x0A)                   /* down arrow */
            {
                if (sel + 1 < PB_SLOTS) { pb_move_selection(sel, sel + 1); sel++; }
                continue;
            }
            if (k >= '1' && k <= '8')        /* number key jumps to that slot */
            {
                unsigned char n = k - '1';
                if (n != sel) { pb_move_selection(sel, n); sel = n; }
                continue;
            }
            if (k == 'q')
                return 0;
            if (k == 'n')                    /* one-shot URL; save offered on return */
            {
                if (!prompt_url())
                    break;
                /* Stash the bare URL before creds and ?term args are spliced in. */
                bare = devicespec;
                if ((bare[0] == 'N' || bare[0] == 'n') && bare[1] == ':')
                    bare += 2;
                if (strlen(bare) < PB_URL_LEN)
                {
                    strcpy(oneshot_url, bare);
                    oneshot_scheme = url_scheme(bare);
                    oneshot_too_long = 0;
                }
                else
                {
                    oneshot_url[0] = 0;
                    oneshot_too_long = 1;
                }
                prompt_creds_and_finalize();
                return 1;
            }
            if (k == 'm')
            {
                monitor_prompt();
                break;                       /* redraw menu with new palette */
            }
            if (k == 'e')
            {
                pb_edit(sel);
                break;                       /* redraw menu */
            }
            if (k == 'd')
            {
                memset(e, 0, sizeof(PBEntry));
                pb_save(sel, e);
                break;                       /* redraw menu */
            }
            if (k == 13)                     /* ENTER -> connect */
            {
                if (!pb_load(sel, e) || !e->url[0])
                    continue;                /* empty slot, ignore */

                set_devicespec_from_url(e->url, e->scheme);

                feed("\x1b[2J\x1b[HCONNECTING TO ");
                feed(e->scheme == SCH_CPM ? cpm_label : (const char *) e->name);
                feed("\r\n");
                screen_flush();

                prompt_creds_and_finalize();
                return 1;
            }
        }
    }
}

/* ---- keyboard output ---- */

/* F1 help. Direct hardware write (bypasses shadow); screen_redraw restores. */
static void show_help(void)
{
    char ver[40];

    screen_overlay_clear();
    screen_overlay_line(0,  "FUJINET VT100 - KEY HELP");
    screen_overlay_line(2,  "ALT + NUMBER = SYMBOL:");
    screen_overlay_line(3,  "  1 [   2 ]   3 {   4 }   5 |");
    screen_overlay_line(4,  "  6 \\   7 _   8 ~   9 `   0 ^");
    screen_overlay_line(5,  "  9 = BACKTICK (SHOWN AS DEGREE)");
    screen_overlay_line(6,  "CTRL + NUMBER = F1 - F10");
    screen_overlay_line(7,  "CTRL + LETTER = CONTROL CODE");
    screen_overlay_line(8,  "CTRL + RIGHT  = TAB");
    screen_overlay_line(9,  "CTRL + LEFT   = BACKSPACE");
    screen_overlay_line(10, "CLEAR         = DEL");
    screen_overlay_line(11, "BREAK         = ESC");
    screen_overlay_line(12, "CTRL + BREAK  = DISCONNECT");

    strcpy(ver, "VERSION: ");
    strcat(ver, VT100_VERSION);
    screen_overlay_line(14, ver);
    screen_overlay_line(16, "PRESS ANY KEY TO RETURN");

    while (isKeyPressed(KEY_PROBE_F1, KEY_BIT_F1)) ;   /* let F1 lift */
    while (inkey()) ;                                  /* drain it */
    while (!inkey()) ;                                 /* any key returns */

    screen_redraw();
}

/* Send arrow as ESC O x (DECCKM set) or ESC [ x (reset). final = A/B/C/D. */
static void send_cursor(char final)
{
    unsigned char seq[3];
    seq[0] = 0x1B;
    seq[1] = screen_appcursor() ? 'O' : '[';
    seq[2] = (unsigned char) final;
    network_write(devicespec, seq, 3);
}

/* F1..F10: ESC O <final>, from the vt100 terminfo kf1..kf10. No F11/F12. */
static const char fkey_final[] = "PQRStuvlwx";

static void send_fkey(unsigned char idx)
{
    unsigned char seq[3];
    seq[0] = 0x1B;
    seq[1] = 'O';
    seq[2] = (unsigned char) fkey_final[idx];
    network_write(devicespec, seq, 3);
}

/* keyboard -> network. CTRL+BREAK quits, F1 shows help, plain arrows send
   DECCKM-aware sequences, CTRL+left/right send BS/TAB, CLEAR sends DEL;
   everything else goes via decode_key. */
static void out_keys(void)
{
    unsigned char k;

    if (isKeyPressed(KEY_PROBE_BREAK, KEY_BIT_BREAK) &&
        isKeyPressed(KEY_PROBE_CTRL, KEY_BIT_CTRL))
    {
        running = 0;
        return;
    }

    if (isKeyPressed(KEY_PROBE_F1, KEY_BIT_F1))
    {
        show_help();
        return;
    }

    k = inkey();
    if (!k)
        return;

    if (isKeyPressed(KEY_PROBE_CTRL, KEY_BIT_CTRL))
    {
        unsigned char b;
        if (k == 0x08) { b = 0x08; network_write(devicespec, &b, 1); return; }  /* CTRL+left  -> BS  */
        if (k == 0x09) { b = 0x09; network_write(devicespec, &b, 1); return; }  /* CTRL+right -> TAB */
        if (k >= '1' && k <= '9') { send_fkey(k - '1'); return; }               /* CTRL+1-9   -> F1-F9 */
        if (k == '0')             { send_fkey(9);       return; }               /* CTRL+0     -> F10 */
    }

    if (k == 0x5E) { send_cursor('A'); return; }   /* up arrow    */
    if (k == 0x0A) { send_cursor('B'); return; }   /* down arrow  */
    if (k == 0x08) { send_cursor('D'); return; }   /* left arrow  */
    if (k == 0x09) { send_cursor('C'); return; }   /* right arrow */
    if (k == 0x0C)                                 /* CLEAR key -> DEL */
    {
        unsigned char del = 0x7F;
        network_write(devicespec, &del, 1);
        return;
    }

    k = decode_key(k);
    if (k)
        network_write(devicespec, &k, 1);
}

/* ---- network input and session lifecycle ---- */

/* Drain FujiNet's buffered bytes into the engine and flush ONCE at the end -
   a flush per read would be a full-screen blit per chunk. Iteration-capped so
   the keyboard still gets serviced during a continuous stream. */
static void in_data(void)
{
    int16_t n;
    unsigned char got = 0;
    unsigned char iters = 0;
    uint16_t sbw;
    uint8_t sconn, serr;
    static unsigned char idle = 0;

    for (;;)
    {
        n = network_read_nb(devicespec, rx_buf, RXSZ);

        if (n < 0)
        {
            running = 0;
            break;
        }
        if (n == 0)
        {
            /* Empty read: poll for EOF every few idle passes (extended error
               136 = remote closed, same as the Atari port). Rate-limited to
               avoid two FujiNet round-trips per loop while idle. */
            if (++idle >= 8)
            {
                idle = 0;
                network_status(devicespec, &sbw, &sconn, &serr);
                if (serr == 136)
                    running = 0;
            }
            break;
        }
        idle = 0;

        vt_write(rx_buf, (uint16_t) n);
        got = 1;

        if (++iters >= 8)                 /* yield to the keyboard */
            break;
    }

    if (got)
        screen_flush();
}

static void connect_and_run(void)
{
    char nb[8];
    uint8_t err;

    err = network_open(devicespec, OPEN_MODE_RW, OPEN_TRANS_NONE);
    if (err != FN_ERR_OK)
    {
        feed("\r\nOPEN ERROR ");
        itoa10(err, nb);
        feed(nb);
        feed(" - PRESS A KEY\r\n");
        screen_flush();
        while (!inkey()) ;
        return;
    }

    feed("\x1b[2J\x1b[H");
    screen_flush();
    term_sendback = net_sendback;
    term_bell = net_bell;

    running = 1;
    while (running)
    {
        in_data();
        out_keys();
    }

    network_close(devicespec);
    term_sendback = 0;
    term_bell = 0;

    /* Clean slate before returning to the menu, especially after CTRL-BREAK. */
    vt100_terminal_reset();
    screen_flush();
}

int main(void)
{
    initCoCoSupport();
    if (!isCoCo3)
    {
        putstr("REQUIRES A COCO 3.\r", 19);
        return 1;
    }

    if (network_init() != FN_ERR_OK)
    {
        putstr("NETWORK INIT FAILED.\r", 21);
        return 1;
    }

    fuji_set_appkey_details(PB_CREATOR, PB_APP, DEFAULT);

    screen_init();
    choose_monitor();

    while (pb_menu())
        connect_and_run();

    screen_shutdown();
    return 0;
}
