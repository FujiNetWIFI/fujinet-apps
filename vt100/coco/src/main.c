/**
 * @brief FujiNet VT100 terminal for the CoCo 3.
 *
 * Mirrors the Atari port's general-terminal structure (prompt for a devicespec,
 * open the N: network device, loop) and the Apple II port's polling read loop
 * that feeds each received byte to the vt100() engine. The Atari's PROCEED
 * interrupt isn't available on the CoCo, so we poll network_status like the
 * Apple II polls sp_bytes_waiting. Transport is fujinet-lib-coco (N: device).
 */

#include <cmoc.h>
#include <coco.h>
#include "vt100.h"
#include "screen.h"
#include "fujinet-network.h"

#define RXSZ 512

extern void (*term_sendback)(char c);   /* set so DSR/cursor replies go to the wire */

static char devicespec[160];
static unsigned char rx_buf[RXSZ];
static unsigned char running;

/* Write a byte buffer to the screen: bulk-write runs of printable ASCII
   straight into the shadow (one call per run) when the decoder is in CHAR
   state, and route control/escape bytes through the engine. This is the hot
   path for both the live network stream and feed(). */
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

/* feed a NUL-terminated stream through the screen */
static void feed(const char *s)
{
    vt_write((const unsigned char *) s, (uint16_t) strlen(s));
}

/* DSR / cursor-position replies from the engine -> back over the connection */
static void net_sendback(char c)
{
    network_write(devicespec, (const unsigned char *) &c, 1);
}

/* ALT + the number row produces the 10 ASCII characters the CoCo keyboard
   lacks. Indexed by (key - '0'):  0='^' 1='[' 2=']' 3='{' 4='}' 5='|' 6='\'
   7='_' 8='~' 9='`'  */
static const char alt_syms[10] = { '^','[',']','{','}','|','\\','_','~','`' };

/* Decode one raw inkey() byte into the single byte to use, applying the
   keyboard scheme by probing modifiers the same way SHIFT is probed:
   ALT+digit -> missing symbol, CTRL+letter -> control code, BREAK -> ESC,
   and the default upper->lower fold (SHIFT keeps caps). The multi-byte arrow
   keys are handled by the caller (out_keys). */
static unsigned char decode_key(unsigned char k)
{
    if (!k)
        return 0;

    if (k == 0x03)                                   /* BREAK alone -> ESC */
        return 0x1B;

    /* ALT is the symbol shift: ONLY ALT+digit produces output. Anything else
       while ALT is held - including the stray code the ALT key itself emits -
       is swallowed (inkey/POLCAT reflects the live matrix, so ALT is still
       held when its own code is read). */
    if (isKeyPressed(KEY_PROBE_ALT, KEY_BIT_ALT))
    {
        if (k >= '0' && k <= '9')
            return alt_syms[k - '0'];
        return 0;
    }

    /* CTRL: ONLY CTRL+letter produces a control code; the CTRL key's own stray
       code (and any other non-letter while CTRL is held) is swallowed. */
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

/* one decoded keystroke (used by the line editor) */
static unsigned char read_key(void)
{
    return decode_key(inkey());
}

/* 80-column line editor: SHIFT-aware input, echoes through the engine, blinking
   block cursor. Returns the line in buf (no newline). */
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

        if (k == 13)                              /* ENTER */
            break;
        if (k == 8)                               /* backspace (left arrow) */
        {
            if (i > 0)
            {
                i--;
                vt100(8);             /* BS only moves left, so erase by */
                screen_putc(' ');     /* overwriting with a space and */
                vt100(8);             /* stepping back over it */
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

/* F1 key reference. Drawn straight to the hardware screen (not the shadow),
   so one re-blit restores the live session. Hit F1 to show, any key to return. */
static void show_help(void)
{
    screen_overlay_clear();
    screen_overlay_line(0,  "FUJINET VT100 - KEY HELP");
    screen_overlay_line(2,  "ALT + NUMBER = SYMBOL:");
    screen_overlay_line(3,  "  1 [   2 ]   3 {   4 }   5 |");
    screen_overlay_line(4,  "  6 \\   7 _   8 ~   9 `   0 ^");
    screen_overlay_line(6,  "CTRL + NUMBER = F1 - F10");
    screen_overlay_line(7,  "CTRL + LETTER = CONTROL CODE");
    screen_overlay_line(8,  "BREAK         = ESC");
    screen_overlay_line(9,  "CLEAR         = TAB");
    screen_overlay_line(10, "CTRL + BREAK  = QUIT TERMINAL");
    screen_overlay_line(12, "PRESS ANY KEY TO RETURN");

    while (isKeyPressed(KEY_PROBE_F1, KEY_BIT_F1)) ;   /* let F1 come back up */
    while (inkey()) ;                                  /* drain it */
    while (!inkey()) ;                                 /* any key returns */

    screen_redraw();                                   /* restore the session */
}

/* Send an arrow key as the sequence the host expects: the application-cursor
   form (ESC O x) when the host turned on DECCKM (mc, vi, etc.), else the normal
   form (ESC [ x). 'final' is one of A/B/C/D. */
static void send_cursor(char final)
{
    unsigned char seq[3];
    seq[0] = 0x1B;
    seq[1] = screen_appcursor() ? 'O' : '[';
    seq[2] = (unsigned char) final;
    network_write(devicespec, seq, 3);
}

/* Function keys F1..F10 (idx 0..9), as the VT100 sends them: ESC O <final>,
   with final from the vt100 terminfo kf1..kf10 (P Q R S t u v l w x). vt100 has
   no F11/F12, so the row stops at F10. */
static const char fkey_final[] = "PQRStuvlwx";

static void send_fkey(unsigned char idx)
{
    unsigned char seq[3];
    seq[0] = 0x1B;
    seq[1] = 'O';
    seq[2] = (unsigned char) fkey_final[idx];
    network_write(devicespec, seq, 3);
}

/* keyboard -> network. CTRL+BREAK quits locally; F1 shows the key help. The
   four arrow keys send VT100 cursor sequences (DECCKM-aware), the CLEAR key
   sends Tab, and the prompt's own editor still uses the left arrow for local
   backspace (term_get_line, separate path). Backspace to the host is CTRL+H.
   Everything else goes through decode_key (ALT/CTRL/SHIFT/BREAK). */
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

    if (k == 0x5E) { send_cursor('A'); return; }   /* up arrow    */
    if (k == 0x0A) { send_cursor('B'); return; }   /* down arrow  */
    if (k == 0x08) { send_cursor('D'); return; }   /* left arrow  */
    if (k == 0x09) { send_cursor('C'); return; }   /* right arrow */
    if (k == 0x0C)                                 /* CLEAR key -> Tab */
    {
        unsigned char tab = 0x09;
        network_write(devicespec, &tab, 1);
        return;
    }

    if (isKeyPressed(KEY_PROBE_CTRL, KEY_BIT_CTRL)) /* CTRL + number row -> F1-F10 */
    {
        if (k >= '1' && k <= '9') { send_fkey(k - '1'); return; }
        if (k == '0')             { send_fkey(9);       return; }
    }

    k = decode_key(k);
    if (k)
        network_write(devicespec, &k, 1);
}

/* network -> engine -> screen. Drain everything FujiNet has buffered, feeding
   it all to the engine, and blit only ONCE afterwards - not once per read.
   (Flushing per read meant a full-screen blit for every small chunk, which is
   what made streaming output crawl.) Capped per call so the keyboard still
   gets serviced during a continuous stream. */
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
        if (n == 0)                       /* nothing buffered right now */
        {
            /* The peer may have hung up with no data left (e.g. ssh `exit`).
               Poll status for disconnect (extended error 136 = EOF/closed,
               same as the Atari port) - but only every few idle passes, so we
               aren't doing two FujiNet round-trips per loop while idle. */
            if (++idle >= 8)
            {
                idle = 0;
                network_status(devicespec, &sbw, &sconn, &serr);
                if (serr == 136)
                    running = 0;
            }
            break;
        }
        idle = 0;                         /* data is flowing */

        vt_write(rx_buf, (uint16_t) n);
        got = 1;

        if (fn_network_error == 136)      /* remote closed */
        {
            running = 0;
            break;
        }
        if (++iters >= 8)                 /* yield to the keyboard periodically */
            break;
    }

    if (got)
        screen_flush();
}

/* one-time monitor-type selection (affects how palette colours are shown) */
static void choose_monitor(void)
{
    unsigned char k;

    feed("\x1b[2J\x1b[HMONITOR TYPE?\r\n\r\n");
    feed("  R = RGB (DEFAULT)\r\n");
    feed("  C = COMPOSITE\r\n");
    screen_flush();

    while ((k = inkey()) == 0) ;
    if (k == 'C' || k == 'c')
        screen_palette(1);
}

static unsigned char get_url(void)
{
    /* The RX buffer is idle while we sit at the connect prompt, so carve the
       scratch buffers out of it rather than spend 352 bytes of permanent BSS.
       devicespec holds the finished URL before the session reuses rx_buf. */
    char *line = (char *) rx_buf;          /*   0..95  (96) */
    char *user = (char *) rx_buf + 96;     /*  96..143 (48) */
    char *pass = (char *) rx_buf + 144;    /* 144..191 (48) */
    char *full = (char *) rx_buf + 192;    /* 192..351 (160) */
    char *sep;
    unsigned int pre;

    feed("\x1b[2J\x1b[HFUJINET VT100 TERMINAL\r\n\r\n");
    feed("DEVICESPEC? (BLANK TO QUIT)\r\n");
    feed("E.G. TELNET://HOST:PORT  (N: ASSUMED)\r\n");
    feed("CTRL-BREAK DISCONNECTS\r\n\r\n");
    feed("ALT+1-0 = [ ] { } | \\ _ ~ ` ^\r\n");
    feed("CTRL+1-0=F1-F10 CTRL+LTR=CODE\r\n");
    feed("BREAK=ESC F1=HELP\r\n\r\n");
    screen_flush();

    term_get_line(line, 96);
    if (line[0] == 0)
        return 0;

    /* assume an "N:" prefix if the user didn't type one */
    if (line[0] == 'N' || line[0] == 'n')
        strcpy(devicespec, line);
    else
    {
        strcpy(devicespec, "N:");
        strcat(devicespec, line);
    }

    /* optional credentials (ssh/ftp/...): the firmware reads user:0@host
       from the URL, so splice them in after the "://" - no special call. */
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

    /* Always tell the host we are a VT100 of our fixed 80x24 size. The firmware
       reads ?term/?cols/?rows for telnet (TTYPE/NAWS) and ssh (pty-req); other
       protocols ignore the query. */
    if (strlen(devicespec) + 28 < sizeof(devicespec))
    {
        strcat(devicespec, strstr(devicespec, "?") ? "&" : "?");
        strcat(devicespec, "term=vt100&cols=80&rows=24");
    }

    return 1;
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

    feed("\x1b[2J\x1b[H");            /* clear for the session */
    screen_flush();
    term_sendback = net_sendback;

    running = 1;
    while (running)
    {
        in_data();
        out_keys();
    }

    network_close(devicespec);
    term_sendback = 0;

    /* clean slate before the prompt returns (esp. after CTRL-BREAK): reset the
       decoder so a half-parsed escape can't eat the clear, default the colors,
       and clear the screen. */
    vt100_reset();
    screen_attr_reset();
    screen_set_reverse(0);            /* undo DECSCNM before touching colours */
    screen_set_region(0, 0);          /* drop any scroll region vi/etc. left set */
    screen_set_autowrap(1);           /* and any DECAWM/DECOM/DECCKM modes */
    screen_set_origin(0);
    screen_set_appcursor(0);
    screen_clear();
    screen_set_pos(0, 0);
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

    screen_init();
    choose_monitor();

    while (get_url())
        connect_and_run();

    width(32);
    return 0;
}
