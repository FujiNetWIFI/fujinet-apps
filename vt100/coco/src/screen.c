/**
 * @brief CoCo 3 80-column screen driver for the vt100 terminal.
 *
 * Shadow lives in MMU-banked block $F4 (paged into slot 1 on demand); the
 * shadow_acquire/release bracket gates ALL access. Static helpers inherit the
 * caller's bracket. Publics that call each other use scroll_{up,down}_raw to
 * avoid nesting. screen_flush bounces row-by-row through blit_scratch.
 *
 * Screen access uses $36 (low 6 bits of $F6): the GIME display register is
 * 6-bit, so on a 2MB DAT-board CoCo3 with 8-bit MMU regs, $F6 would select a
 * block GIME isn't displaying. $36 hits the right block on 128K/512K/2MB.
 */

#include <cmoc.h>
#include <coco.h>
#include "screen.h"

#define COLS     80
#define ROWS     24
#define STRIDE   (COLS * 2)
#define BUFSZ    (ROWS * STRIDE)
#define BG_COLOR 0               /* 6-bit colour code for the border (black) */
#define DEF_ATTR 0x38            /* fg slot 7 (white) / bg slot 0 (black) */

#define VT_SHADOW ((unsigned char *)0x2000)   /* valid only inside the bracket */

/* The CoCo3 80-col font carries real caret and underscore glyphs, but at
   shifted code points: caret at 0x60, underscore at 0x7F. It has no backtick
   glyph at all (0x60 is the caret), so we show an incoming backtick as the
   degree symbol (glyph 0x1E) to keep it distinct from a true caret. Remap on
   DISPLAY only - the wire and stored strings always keep true ASCII. */
#define CARET_ASCII  0x5E
#define CARET_GLYPH  0x60
#define USCORE_ASCII 0x5F
#define USCORE_GLYPH 0x7F
#define BTICK_ASCII  0x60
#define BTICK_GLYPH  0x1E

unsigned char blit_scratch[STRIDE];

static void shadow_acquire(void)
{
    asm
    {
        orcc    #$50
        lda     #$F4
        sta     $FFA1
    }
}

static void shadow_release(void)
{
    asm
    {
        lda     #$F9
        sta     $FFA1
        andcc   #$AF
    }
}

unsigned char _row = 0;
unsigned char _col = 0;

static unsigned char _attr = DEF_ATTR;
static unsigned char _cursor_on = 1;

/* Scrolling region [_top.._bot], inclusive (DECSTBM). */
static unsigned char _top = 0;
static unsigned char _bot = ROWS - 1;

static unsigned char _autowrap = 1;       /* DECAWM */
static unsigned char _origin = 0;         /* DECOM */
static unsigned char _reverse = 0;        /* DECSCNM */
static unsigned char _def_attr = DEF_ATTR;
static unsigned char _appcursor = 0;      /* DECCKM - read by the keyboard layer */

/* Pending-wrap (last-column flag). Cursor sits ON the last column after a
   glyph is written there; the wrap happens on the NEXT glyph. */
static unsigned char _pending = 0;

/* set_cell / blank_cells require the caller to hold shadow_acquire. */
static void set_cell(unsigned int o, unsigned char ch, unsigned char at)
{
    VT_SHADOW[o]     = ch;
    VT_SHADOW[o + 1] = at;
}

static unsigned int cell_off(unsigned char x, unsigned char y)
{
    return (unsigned int) y * STRIDE + (unsigned int) x * 2;
}

/* Dirty range for screen_flush. dmin > dmax means nothing dirty. */
static unsigned char dmin = ROWS;
static unsigned char dmax = 0;
static unsigned char last_cur_row = 0;

unsigned int row_off;                       /* read by the per-row blit asm */

static void mark_dirty(unsigned char row)
{
    if (row >= ROWS)
        return;
    if (row < dmin) dmin = row;
    if (row > dmax) dmax = row;
}

/* Caller must hold shadow_acquire. */
static void blank_cells(unsigned char x, unsigned char y, unsigned char n)
{
    unsigned char i;

    if (y >= ROWS)
        return;
    for (i = 0; i < n && (unsigned char) (x + i) < COLS; i++)
        set_cell(cell_off(x + i, y), ' ', _def_attr);
    mark_dirty(y);
}

/* ---- scrolling ----
   The _raw versions are for callers that already hold shadow_acquire. */
static void scroll_up_raw(void)
{
    memmove(VT_SHADOW + (unsigned int) _top * STRIDE,
            VT_SHADOW + (unsigned int) (_top + 1) * STRIDE,
            (unsigned int) (_bot - _top) * STRIDE);
    blank_cells(0, _bot, COLS);
    mark_dirty(_top);
}

static void scroll_down_raw(void)
{
    memmove(VT_SHADOW + (unsigned int) (_top + 1) * STRIDE,
            VT_SHADOW + (unsigned int) _top * STRIDE,
            (unsigned int) (_bot - _top) * STRIDE);
    blank_cells(0, _top, COLS);
    mark_dirty(_bot);
}

void screen_scroll_up(void)
{
    shadow_acquire();
    scroll_up_raw();
    shadow_release();
}

void screen_scroll_down(void)
{
    shadow_acquire();
    scroll_down_raw();
    shadow_release();
}

/* DECSTBM: top/bottom are 1-based; 0 means default. Homes the cursor. */
void screen_set_region(unsigned char top, unsigned char bottom)
{
    if (bottom == 0 || bottom > ROWS) bottom = ROWS;
    if (top == 0) top = 1;
    if (top >= bottom) { top = 1; bottom = ROWS; }
    _top = top - 1;
    _bot = bottom - 1;
    _pending = 0;
    _col = 0;
    _row = _origin ? _top : 0;
}

/* Reverse index: up one row, scrolling the region down at the top margin. */
void screen_ri(void)
{
    _pending = 0;
    if (_row == _top)
    {
        shadow_acquire();
        scroll_down_raw();
        shadow_release();
    }
    else if (_row > 0)
        _row--;
}

void screen_set_autowrap(unsigned char on) { _autowrap = on; }     /* DECAWM */

/* DECOM: cursor addressing relative to region (set) or absolute (reset). */
void screen_set_origin(unsigned char on)
{
    _origin = on;
    _pending = 0;
    _col = 0;
    _row = on ? _top : 0;
}

void screen_set_appcursor(unsigned char on) { _appcursor = on; }   /* DECCKM */
unsigned char screen_appcursor(void)        { return _appcursor; }

/* DECSCNM: swap fg/bg of every existing cell + the default attribute.
   Palette slots 0-7 (bg) and 8-15 (fg) hold the same 8 colours, so swapping
   the 3-bit attribute fields swaps the displayed colours. */
void screen_set_reverse(unsigned char on)
{
    unsigned int o;
    unsigned char a;

    if ((on != 0) == (_reverse != 0))
        return;
    _reverse = on;

    _attr     = (_attr     & 0xC0) | ((_attr     & 0x07) << 3) | ((_attr     >> 3) & 0x07);
    _def_attr = (_def_attr & 0xC0) | ((_def_attr & 0x07) << 3) | ((_def_attr >> 3) & 0x07);

    shadow_acquire();
    for (o = 1; o < BUFSZ; o += 2)
    {
        a = VT_SHADOW[o];
        VT_SHADOW[o] = (a & 0xC0) | ((a & 0x07) << 3) | ((a >> 3) & 0x07);
    }
    shadow_release();

    mark_dirty(0);
    mark_dirty(ROWS - 1);
}

void screen_putc(unsigned char c)
{
    unsigned int o;

    shadow_acquire();

    if (_pending)                     /* finish wrap armed by the previous glyph */
    {
        _pending = 0;
        if (_autowrap)
        {
            _col = 0;
            if (_row == _bot)
                scroll_up_raw();
            else if (_row < ROWS - 1)
                _row++;
        }
    }

    o = (unsigned int) _row * STRIDE + (unsigned int) _col * 2;
    if (c == CARET_ASCII)       c = CARET_GLYPH;
    else if (c == USCORE_ASCII) c = USCORE_GLYPH;
    else if (c == BTICK_ASCII)  c = BTICK_GLYPH;
    VT_SHADOW[o]     = c;
    VT_SHADOW[o + 1] = _attr;
    if (_row < dmin) dmin = _row;
    if (_row > dmax) dmax = _row;

    if (_col >= COLS - 1)
        _pending = 1;
    else
        _col++;

    shadow_release();
}

/* Bulk-write a run of printable ASCII to the shadow. Caller must guarantee
   the decoder is in CHAR state and every byte is printable. */
void screen_puts_run(const unsigned char *buf, unsigned int len)
{
    unsigned char col = _col;
    unsigned char row = _row;
    unsigned char attr = _attr;
    unsigned char *p;

    shadow_acquire();

    if (row < dmin) dmin = row;
    if (row > dmax) dmax = row;
    p = VT_SHADOW + (unsigned int) row * STRIDE + (unsigned int) col * 2;

    while (len)
    {
        if (_pending)
        {
            _pending = 0;
            if (_autowrap)
            {
                col = 0;
                if (row == _bot)
                {
                    _col = 0;
                    _row = row;
                    scroll_up_raw();
                }
                else if (row < ROWS - 1)
                    row++;
            }
        }

        if (row < dmin) dmin = row;
        if (row > dmax) dmax = row;

        /* inner run: write up to (not including) the last column */
        p = VT_SHADOW + (unsigned int) row * STRIDE + (unsigned int) col * 2;
        while (len && col < COLS - 1)
        {
            unsigned char ch = *buf++;
            if (ch == CARET_ASCII)       ch = CARET_GLYPH;
            else if (ch == USCORE_ASCII) ch = USCORE_GLYPH;
            else if (ch == BTICK_ASCII)  ch = BTICK_GLYPH;
            *p++ = ch;
            *p++ = attr;
            col++;
            len--;
        }

        if (len && col == COLS - 1)          /* last column: write, then arm wrap */
        {
            unsigned char ch = *buf++;
            if (ch == CARET_ASCII)       ch = CARET_GLYPH;
            else if (ch == USCORE_ASCII) ch = USCORE_GLYPH;
            else if (ch == BTICK_ASCII)  ch = BTICK_GLYPH;
            *p++ = ch;
            *p = attr;
            _pending = 1;
            len--;
        }
    }

    _col = col;
    _row = row;

    shadow_release();
}

/* ---- cursor motion (clamped, no wrap). Each clears the pending-wrap flag. ---- */
void screen_cr(void) { _col = 0; _pending = 0; }

void screen_lf(void)
{
    _pending = 0;
    if (_row == _bot)
    {
        shadow_acquire();
        scroll_up_raw();
        shadow_release();
    }
    else if (_row < ROWS - 1)
        _row++;
}

/* VT100 BS is non-destructive: move left, stop at the left margin. The host
   handles erasure (BS space BS). */
void screen_bs(void)
{
    _pending = 0;
    if (_col > 0)
        _col--;
}

/* Tab stops: 1 = stop. HTS sets, TBC clears, HT advances. */
static unsigned char _tabstop[COLS];

static void screen_default_tabs(void)
{
    unsigned char i;
    for (i = 0; i < COLS; i++)
        _tabstop[i] = (i && (i % 8 == 0)) ? 1 : 0;
}

void screen_tab(void)
{
    unsigned char c;
    _pending = 0;
    for (c = _col + 1; c < COLS; c++)
        if (_tabstop[c]) { _col = c; return; }
    _col = COLS - 1;
}

void screen_set_tab(void)        { if (_col < COLS) _tabstop[_col] = 1; }
void screen_clear_tab(void)      { if (_col < COLS) _tabstop[_col] = 0; }
void screen_clear_all_tabs(void) { unsigned char i; for (i = 0; i < COLS; i++) _tabstop[i] = 0; }

void screen_cursor_right(unsigned char n)
{
    _pending = 0;
    if (!n) n = 1;
    if ((unsigned int) _col + n >= COLS)
        _col = COLS - 1;
    else
        _col += n;
}

void screen_cursor_left(unsigned char n)
{
    _pending = 0;
    if (!n) n = 1;
    _col = (n > _col) ? 0 : _col - n;
}

void screen_cursor_up(unsigned char n)
{
    unsigned char limit = (_row >= _top) ? _top : 0;
    unsigned char t;
    _pending = 0;
    if (!n) n = 1;
    t = (n > _row) ? 0 : (unsigned char) (_row - n);
    _row = (t < limit) ? limit : t;
}

void screen_cursor_down(unsigned char n)
{
    unsigned char limit = (_row <= _bot) ? _bot : (ROWS - 1);
    _pending = 0;
    if (!n) n = 1;
    if ((unsigned int) _row + n > limit)
        _row = limit;
    else
        _row += n;
}

void screen_set_pos(unsigned char x, unsigned char y)
{
    _pending = 0;
    _col = (x < COLS) ? x : (COLS - 1);
    if (_origin)                              /* DECOM: row relative to region */
    {
        y = (unsigned char) (y + _top);
        if (y > _bot)
            y = _bot;
    }
    _row = (y < ROWS) ? y : (ROWS - 1);
}

void screen_get_pos(unsigned char *row, unsigned char *col)
{
    *row = _row;
    *col = _col;
}

/* ---- clearing (blanked cells get _def_attr) ---- */
void screen_clear(void)
{
    unsigned char y;
    shadow_acquire();
    for (y = 0; y < ROWS; y++)
        blank_cells(0, y, COLS);
    shadow_release();
}

void screen_clear_to_end_of_line(void)
{
    shadow_acquire();
    blank_cells(_col, _row, COLS - _col);
    shadow_release();
}

void screen_clear_current_line(void)
{
    shadow_acquire();
    blank_cells(0, _row, COLS);
    shadow_release();
}

/* EL 1: start of line to cursor. */
void screen_clear_line_to_cursor(void)
{
    shadow_acquire();
    blank_cells(0, _row, _col + 1);
    shadow_release();
}

/* ED 1: top-left to cursor (mirror of cursor_to_end). */
void screen_clear_beg_to_cursor(void)
{
    unsigned char r;
    shadow_acquire();
    for (r = 0; r < _row; r++)
        blank_cells(0, r, COLS);
    blank_cells(0, _row, _col + 1);
    shadow_release();
}

void screen_clear_cursor_to_end(void)
{
    unsigned char r;
    shadow_acquire();
    blank_cells(_col, _row, COLS - _col);
    for (r = _row + 1; r < ROWS; r++)
        blank_cells(0, r, COLS);
    shadow_release();
}

/* ---- insert / delete line ---- */
void screen_insert_line(unsigned char n)
{
    unsigned char j, r;
    if (!n) n = 1;
    shadow_acquire();
    for (j = 0; j < n; j++)
    {
        for (r = ROWS - 1; r > _row; r--)
            memcpy(VT_SHADOW + (unsigned int) r * STRIDE,
                   VT_SHADOW + (unsigned int) (r - 1) * STRIDE, STRIDE);
        blank_cells(0, _row, COLS);
    }
    shadow_release();
    mark_dirty(_row);
    mark_dirty(ROWS - 1);
}

void screen_delete_line(unsigned char n)
{
    unsigned char j, r;
    if (!n) n = 1;
    shadow_acquire();
    for (j = 0; j < n; j++)
    {
        for (r = _row; r < ROWS - 1; r++)
            memcpy(VT_SHADOW + (unsigned int) r * STRIDE,
                   VT_SHADOW + (unsigned int) (r + 1) * STRIDE, STRIDE);
        blank_cells(0, ROWS - 1, COLS);
    }
    shadow_release();
    mark_dirty(_row);
    mark_dirty(ROWS - 1);
}

/* ---- attributes ---- */
void screen_attr_reset(void)     { _attr = _def_attr; }
void screen_attr_underline(void) { _attr |= 0x40; }
void screen_attr_blink(void)     { _attr |= 0x80; }

void screen_attr_inverse(void)
{
    _attr = (_attr & 0xC0) | ((_attr & 0x07) << 3) | ((_attr >> 3) & 0x07);
}

void screen_attr_invisible(void)
{
    unsigned char bg = _attr & 0x07;
    _attr = (_attr & 0xC0) | (bg << 3) | bg;     /* fg = bg */
}


void screen_set_fg(unsigned char c) { _attr = (_attr & 0xC7) | ((c & 7) << 3); }
void screen_set_bg(unsigned char c) { _attr = (_attr & 0xF8) |  (c & 7); }

void screen_show_cursor(unsigned char on)
{
    _cursor_on = on;
}

/* ---- DECSC / DECRC ---- */
static unsigned char sc_row, sc_col, sc_attr;

void screen_save_cursor(void)
{
    sc_row = _row;
    sc_col = _col;
    sc_attr = _attr;
}

void screen_restore_cursor(void)
{
    _row = sc_row;
    _col = sc_col;
    _attr = sc_attr;
    _pending = 0;
}

/* DECALN: fill the screen with 'E', home the cursor. */
void screen_decaln(void)
{
    unsigned int o;
    shadow_acquire();
    for (o = 0; o < BUFSZ; o += 2)
    {
        VT_SHADOW[o]     = 'E';
        VT_SHADOW[o + 1] = _def_attr;
    }
    shadow_release();
    _row = 0;
    _col = 0;
    mark_dirty(0);
    mark_dirty(ROWS - 1);
}

/* ---- transient overlay (F1 help) ----
   Writes straight to the 80-col screen, bypassing the shadow. screen_redraw
   restores the live session afterwards. */
unsigned int ov_src, ov_dst;
unsigned char ov_attr;

void screen_overlay_clear(void)
{
    ov_attr = DEF_ATTR;
    asm
    {
        orcc    #$50
        lda     #$36            ; screen
        sta     $FFA1
        ldy     #$2000
        ldb     _ov_attr
@ovc
        lda     #$20            ; space
        sta     ,y+
        stb     ,y+
        cmpy    #$2F00          ; $2000 + 24*160
        blo     @ovc
        lda     #$F9
        sta     $FFA1
        andcc   #$AF
    }
}

void screen_overlay_line(unsigned char row, const char *s)
{
    ov_src = (unsigned int) s;
    ov_dst = 0x2000 + (unsigned int) row * STRIDE;
    ov_attr = DEF_ATTR;
    asm
    {
        orcc    #$50
        lda     #$36            ; screen
        sta     $FFA1
        ldx     _ov_src
        ldy     _ov_dst
        ldb     _ov_attr
@ovl
        lda     ,x+
        beq     @ovld
        cmpa    #$5E            ; '^' -> caret glyph 0x60
        bne     @ov1
        lda     #$60
        bra     @ovw
@ov1
        cmpa    #$5F            ; '_' -> underscore glyph 0x7F
        bne     @ov2
        lda     #$7F
        bra     @ovw
@ov2
        cmpa    #$60            ; '`' -> degree glyph 0x1E
        bne     @ovw
        lda     #$1E
@ovw
        sta     ,y+
        stb     ,y+
        bra     @ovl
@ovld
        lda     #$F9
        sta     $FFA1
        andcc   #$AF
    }
}

/* Re-blit the entire shadow (used to clear an overlay). */
void screen_redraw(void)
{
    mark_dirty(0);
    mark_dirty(ROWS - 1);
    screen_flush();
}

/* shadow -> bounce -> screen for one row, IRQs masked throughout.
   X/Y only: U is cmoc's frame pointer and must not be clobbered. */
static void flush_row(unsigned char r)
{
    row_off = 0x2000 + (unsigned int) r * STRIDE;
    asm
    {
        orcc    #$50
        ; map shadow into slot 1
        lda     #$F4
        sta     $FFA1
        ; shadow row -> blit_scratch
        ldx     _row_off
        ldy     #_blit_scratch
@in
        ldd     ,x++
        std     ,y++
        cmpy    #_blit_scratch+160
        blo     @in
        ; map screen into slot 1
        lda     #$36            ; screen
        sta     $FFA1
        ; blit_scratch -> screen row
        ldx     #_blit_scratch
        ldy     _row_off
@out
        ldd     ,x++
        std     ,y++
        cmpx    #_blit_scratch+160
        blo     @out
        ; restore
        lda     #$F9
        sta     $FFA1
        andcc   #$AF
    }
}

/* Blit dirty rows + draw the block cursor by inverting one cell's attribute. */
void screen_flush(void)
{
    unsigned int co = 0;
    unsigned char saved = 0;
    unsigned char have_cursor;
    unsigned char r;

    /* mark the cursor's row and its previous row so the block follows it */
    mark_dirty(last_cur_row);
    if (_row < ROWS)
        mark_dirty(_row);

    if (dmin > dmax)
        return;

    have_cursor = _cursor_on && (_col < COLS && _row < ROWS);
    if (have_cursor)
    {
        co = cell_off(_col, _row) + 1;        /* attr byte */
        shadow_acquire();
        saved = VT_SHADOW[co];
        VT_SHADOW[co] = (saved & 0xC0) | ((saved & 0x07) << 3) | ((saved >> 3) & 0x07);
        shadow_release();
    }

    for (r = dmin; r <= dmax; r++)
        flush_row(r);

    if (have_cursor)
    {
        shadow_acquire();
        VT_SHADOW[co] = saved;
        shadow_release();
    }

    dmin = ROWS;
    dmax = 0;
    last_cur_row = (_row < ROWS) ? _row : (ROWS - 1);
}

/* Load the 8 ANSI colours into palette slots 0-7 (bg) and 8-15 (fg). */
void screen_palette(unsigned char composite)
{
    if (composite)
        cmp();
    else
        rgb();
    paletteRGB(0, 0, 0, 0);  paletteRGB(8,  0, 0, 0);   /* black   */
    paletteRGB(1, 3, 0, 0);  paletteRGB(9,  3, 0, 0);   /* red     */
    paletteRGB(2, 0, 3, 0);  paletteRGB(10, 0, 3, 0);   /* green   */
    paletteRGB(3, 3, 3, 0);  paletteRGB(11, 3, 3, 0);   /* yellow  */
    paletteRGB(4, 0, 0, 3);  paletteRGB(12, 0, 0, 3);   /* blue    */
    paletteRGB(5, 3, 0, 3);  paletteRGB(13, 3, 0, 3);   /* magenta */
    paletteRGB(6, 0, 3, 3);  paletteRGB(14, 0, 3, 3);   /* cyan    */
    paletteRGB(7, 3, 3, 3);  paletteRGB(15, 3, 3, 3);   /* white   */
    setBorderColor(BG_COLOR);
}

void screen_init(void)
{
    width(80);
    screen_palette(0);          /* default to RGB */
    _attr = DEF_ATTR;
    _top = 0;
    _bot = ROWS - 1;
    _autowrap = 1;
    _origin = 0;
    _reverse = 0;
    _def_attr = DEF_ATTR;
    _appcursor = 0;
    _pending = 0;
    screen_default_tabs();
    _row = 0;
    _col = 0;
    screen_clear();
    screen_flush();
}

void screen_shutdown(void)
{
    resetPalette(1);             /* restore the CoCo 3 RGB text palette */
    rgb();
    width(32);
    cls(255);                    /* CLS with no colour argument */
}
