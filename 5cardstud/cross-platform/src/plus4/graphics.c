#ifdef __PLUS4__

/*
  Ported graphics functionality from C64 (VIC-II) to C16/Plus4 (TED).
  Notes:
   - TED on the Plus/4 does NOT have hardware sprites like the VIC-II.
     Sprite-using code has been replaced by a simple character/bitmap
     emulation. If you want per-pixel lines you will need to implement
     a TED bitmap draw routine (or use character-cell tricks).
   - The hardware abstraction functions (ted_hw_*) contain TODOs and
     placeholders for the correct TED register writes. Verify register
     addresses for your cc65 headers or runtime.
*/

#include <conio.h>
#include <peekpoke.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "charset.h"
#include "../platform-specific/graphics.h"

/* Platform constants — adjust these to match your Plus/4 memory layout.
   The values below are reasonable defaults but must be verified on your
   build (Kernal, charset placement, and screen addresses differ). */

/* Screen & charset targets (example choices) */
#define CHARSET_LOC_P4  0xD000  /* TODO: adjust if you place charset elsewhere */
#define DBLBUF_LOC_P4   0xD800  /* double buffer character memory */
#define SCREEN_LOC_P4   0xE000  /* visible screen memory address (example) */
#define SPRITE_LOC_P4   0xE400  /* not real sprite RAM on TED — used for emu */

#define CHARSET_LOC CHARSET_LOC_P4
#define DBLBUF_LOC  DBLBUF_LOC_P4
#define SCREEN_LOC  SCREEN_LOC_P4
#define SPRITE_LOC  SPRITE_LOC_P4

/* Multi Colors - re-use your original color constants; they depend on
   how you map 'color numbers' to TED registers. */
#define COL_RED    (8 + COLOR_RED)
#define COL_BLACK  (8 + COLOR_BLACK)
#define COL_WHITE  (8 + COLOR_WHITE)
#define COL_YELLOW (8 + COLOR_YELLOW)

/* Keep original names for compatibility */
#define SCR (unsigned char*)DBLBUF_LOC
#define SCR_STATUS (SCR+40*23)
#define SCR_STATUS_DIRECT (unsigned char*)(SCREEN_LOC+40*23)

/* Set text color macro — implemented via hardware layer below */
#define SET_COL(val) ted_hw_set_text_color((val))

/* Abstraction layer for TED-specific operations
   The functions below should be adjusted to talk to the Plus/4 TED registers
   (or to memory areas) that your target system exposes. */

/* --- Hardware abstraction prototypes --- */
static void ted_hw_wait_vsync(void);
static void ted_hw_set_text_color(unsigned char v);
static void ted_hw_set_charset(unsigned int addr);
static void ted_hw_set_screen(unsigned int addr);
static void ted_hw_set_bank(unsigned int addr);
static void ted_hw_set_border_bg_colors(unsigned char border, unsigned char bg0, unsigned char bg1, unsigned char bg2);
static void ted_hw_write_color_ram(unsigned char *src, unsigned int len);
static void ted_hw_disable_shift_charset(void);
static void ted_hw_enable_shift_charset(void);

/* --- Simple fallback for "sprite" drawing on TED ---
   TED lacks VIC-style sprites. For things that previously used sprites
   (cursor/line visual), we emulate by writing character patterns into a
   reserved area of the charset and placing those characters on-screen.
   This is slower but portable. */
static void emu_clear_sprite_area(void);
static void emu_draw_line_chars(unsigned char x, unsigned char y, unsigned char w);
static void emu_hide_line_chars(void);

/* --- Implementation (portable skeleton) --- */

/* Some state variables */
static unsigned short screen = SCREEN_LOC;
static unsigned char col_text = COL_BLACK;

bool always_render_full_cards = 0;
unsigned char colorMode = 0;

unsigned char cycleNextColor() {
  setColorMode((colorMode + 1) % 3);
  return colorMode;
}

void setColorMode(unsigned char mode) {
  colorMode = mode;
  ted_hw_wait_vsync();
  if (colorMode == 0) {
    col_text = COL_BLACK;
    /* map to TED bg/border registers */
    ted_hw_set_border_bg_colors(COLOR_BLACK, COLOR_GREEN, COLOR_BLUE, COLOR_WHITE);
  } else if (colorMode == 1) {
    col_text = COL_WHITE;
    ted_hw_set_border_bg_colors(COLOR_BLACK, COLOR_BLUE, COLOR_GRAY2, COLOR_WHITE);
  } else {
    col_text = COL_WHITE;
    ted_hw_set_border_bg_colors(COLOR_BLACK, COLOR_GRAY1, COLOR_GRAY3, COLOR_WHITE);
  }
}

void enableDoubleBuffer() {
  /* On the Plus/4 we point the visible screen to SCREEN_LOC and render to DBLBUF_LOC,
     or we can make the kernel use a different screen page for cputs. */
  ted_hw_set_screen(SCREEN_LOC);

  screen = DBLBUF_LOC;

  ted_hw_wait_vsync();

  /* The C64 POKE(648, bank) trick sets the kernal screen pointer.
     On Plus/4 you may need a different POKE — adjust here if needed. */
  /* TODO: if your environment/cc65 runtime supports a kernel page-set for cputs,
           implement it below. For now we leave as-is. */
}

void disableDoubleBuffer() {
  /* Make the double-buffer become the visible screen (if you want that) */
  ted_hw_set_screen(DBLBUF_LOC);
  screen = SCREEN_LOC;
}

void clearStatusBar() {
  memset(screen + 920, 0x80, 80);
}

void drawBuffer() {
  ted_hw_wait_vsync();
  memcpy((void*)SCREEN_LOC, (void*)DBLBUF_LOC, 1024);
}

void drawStatusTextAt(unsigned char x, char* s) {
  static unsigned char j, split;
  SET_COL(COL_BLACK);
  split = strlen(s) > 40;

  while (j = *s++ & 0x7F) {
    if (j >= 97 && j <= 122) j -= 32;
    POKE(SCR_STATUS + x, j + (j < 0x40 ? j == 0x20 ? 0x60 : 0x80 : j > 0xC0 ? -0x40 : 0x40));
    POKE(SCR_STATUS_DIRECT + x, j + (j < 0x40 ? j == 0x20 ? 0x60 : 0x80 : j > 0xC0 ? -0x40 : 0x40));
    ++x;

    if (split && j == ',' && x < 40) {
      if (*s == ' ') ++s;
      x = 40;
    }
  }
}

void drawStatusText(char* s) {
  clearStatusBar();
  drawStatusTextAt(0, s);
}

void drawStatusTimer() {
  POKE(screen + 920 + 39, 0x53);
}

void drawText(unsigned char x, unsigned char y, const char* s) {
  unsigned char i, c;
  char tmp[128];

  strcpy(tmp, s);
  SET_COL(col_text);

  for (i = strlen(tmp) - 1; i < 255; i--) {
    c = tmp[i];
    if (c >= 97 && c <= 122) tmp[i] -= 32;
    else if (c >= 193) tmp[i] -= 128;
  }

  cputsxy(x, y, tmp);
}

/* resetScreen adapted for the Plus/4 screen memory layout (assumes 40x25) */
void resetScreen() {
  /* Clear screen except for corners */
  memset(SCR + 1, 0, 38);
  memset(SCR + 40, 0, 40 * 21);
  memset(SCR + 40 * 22 + 1, 0, 38);

  /* Draw rounded table corners (re-using the same character codes as before) */
  POKE(SCR + 0, 86);
  POKE(SCR + 39, 87);
  POKE(SCR + 40 * 22, 88);
  POKE(SCR + 40 * 23 - 1, 85);
}

/* drawCard mostly unchanged — coloring uses TED color RAM via ted_hw_write_color_ram */
void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, unsigned char isHidden) {
  static unsigned char i;
  int suit, val, space, top, bottom;
  unsigned char col;
  unsigned char* loc;

  top = 0x7776;
  space = isHidden ? 0x605F : 0x63A1;
  bottom = 0x7978;

  val = 0x6300;
  i = s[0];
  if (i >= 97 && i <= 122) i -= 32;

  switch (i) {
    case 't': val = 0xABAA; break;
    case 'j': val += 0x9c; break;
    case 'q': val += 0x9d; break;
    case 'k': val += 0x9e; break;
    case 'a': val += 0x9f; break;
    case 63:
      suit = space = val = 0x6261;
      top += 0x404;
      bottom += 0x404;
      break;
    default:
      val += i + 0x70;
  }

  i = s[1];
  if (i >= 97 && i <= 122) i -= 32;

  switch (i) {
    case 'h': suit = 0x6665; col = COL_RED; break;
    case 'd': suit = 0x6867; col = COL_RED; break;
    case 'c': suit = 0x6A69; col = COL_BLACK; break;
    case 's': suit = 0x6C6B; col = COL_BLACK; break;
    default: col = COL_BLACK;
  }

  /* Color the card: write to TED color RAM location if available.
     On the Plus/4 the color RAM mapping is different; this is the logic
     placeholder to write color bytes for the 3 rows of the card. */
  loc = (unsigned char*)(COLOR_RAM + y * 40 + x + 40);
  if (partial & 1) {
    POKE((unsigned)loc, col);
    POKE((unsigned)loc + 40, COL_BLACK);
    POKE((unsigned)loc + 80, col);
  }
  if (partial & 2) {
    POKE((unsigned)loc + 1, col);
    POKE((unsigned)loc + 41, COL_BLACK);
    POKE((unsigned)loc + 81, col);
  }

  loc = (unsigned char*)SCR + y * 40 + x;

  /* Top of card */
  POKEW(loc, top);
  loc += 39;

  /* Draw left border char only if not overlaying an existing card */
  if (!PEEK((unsigned)loc)) {
    POKE((unsigned)loc, 0x6e);
    POKE((unsigned)loc + 40, 0x6e);
    POKE((unsigned)loc + 80, 0x6e);
  }

  POKEW(++loc, val);
  POKEW(loc += 40, space);
  POKEW(loc += 40, suit);
  POKEW(loc += 40, bottom);
}

void drawChip(unsigned char x, unsigned char y) {
  SET_COL(COL_RED);
  cputcxy(x, y, 'J');
}

/* drawLine: replace hardware-sprite drawing with a character-based emulation.
   If w>8 and within text area, we draw repeated characters as before. For
   longer or outside-range lines we emulate "sprite" by writing reserved chars. */
void drawLine(unsigned char x, unsigned char y, unsigned char w) {
  static unsigned int xx;

  if (w > 8 && y < 23) {
    SET_COL(col_text);
    cputcxy(x, y, 'A');
    while (--w) cputc('A');
  } else {
    /* Sprite emulation: draw characters into a reserved area and place them */
    emu_draw_line_chars(x, y, w);
  }
}

void hideLine(unsigned char x, unsigned char y, unsigned char w) {
  emu_hide_line_chars();
}

/* drawBox unchanged except for color handling via SET_COL */
void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h) {
  static unsigned char i;
  SET_COL(COL_YELLOW);
  --w;
  cputcxy(x, y, ';');
  for (i = w; i < 255; i--) cputc('A');
  cputc('<');

  for (i = y + h; i > y; i--) {
    cputcxy(x, i, '?');
    cputcxy(x + w + 2, i, '?');
  }

  i = y + h + 1;
  cputcxy(x, i, '=');
  for (i = w; i < 255; i--) cputc('A');
  cputc('>');
}

void drawBorder() {
  static unsigned char i;
  for (i = 0; i < 32; i += 4) {
    SET_COL(COL_RED);
    cputsxy(4 + i, 1, "UV");
    cputsxy(4 + i, 21, "UV");

    SET_COL(COL_BLACK);
    cputsxy(6 + i, 1, "WX");
    cputsxy(6 + i, 21, "WX");
  }

  drawCard(1, 0, FULL_CARD, "as", 0);
  drawCard(37, 0, FULL_CARD, "ah", 0);
  drawCard(1, 18, FULL_CARD, "ad", 0);
  drawCard(37, 18, FULL_CARD, "ac", 0);
}

void drawLogo() {
  static unsigned char i;
  i = 4;
  SET_COL(col_text);
  cputsxy(18, i, "FEF");
  cputsxy(13, ++i, "fuji");

  SET_COL(COL_BLACK);
  cputs("HEJEB");

  SET_COL(col_text);
  cputs("net");
  cputsxy(18, ++i, "FFEB");
  cputsxy(19, ++i, "GFF");

  cputsxy(17, i + 2, "5 card stud");
}

void resetGraphics() {
  /* TED doesn't have VIC.spr_exp_x / spr_ena; we skip sprite resets. */

  ted_hw_wait_vsync();

  /* Return screen/bank to initial values (HAL handles TED specifics) */
  //ted_hw_set_bank(0);     /* TODO: set appropriate bank */
  ted_hw_set_screen(SCREEN_LOC);
  ted_hw_set_charset(CHARSET_LOC);

  /* Reset colors: write to color RAM (size & mapping differ on Plus/4) */
  /* We'll fill 1000 color cells like original logic — adjust if needed */
  //ted_hw_write_color_ram((unsigned char*)0x0E /* color value placeholder */, 1000);

  /* Border and background defaults via HAL */
  //ted_hw_set_border_bg_colors(COLOR_BLACK, COLOR_GREEN, COLOR_BLUE, COLOR_WHITE);

  //ted_hw_enable_shift_charset(); /* placeholder */
}

/* initGraphics: load charset and prepare double buffer */
void initGraphics() {
  ted_hw_disable_shift_charset();

  TED.hscroll = 0x18; // Enable multicolor mode, 40 columns

  /* Clear double buffer */
  memset((void*)DBLBUF_LOC, 0, 1000);
  memset((void*)(DBLBUF_LOC + 920), 0x80, 80);

  /* Load custom charset into CHARSET_LOC (verify addressing) */
  memcpy((void*)CHARSET_LOC, &charset, 2048);
  ted_hw_set_charset(CHARSET_LOC);

  //ted_hw_set_bank(CHARSET_LOC);
  memset((void*)COLOR_RAM, 6, 1000); /* adjust COLOR_RAM mapping if needed */

  enableDoubleBuffer();

  /* Set colors via HAL */
  ted_hw_set_border_bg_colors(COLOR_BLACK, COLOR_GREEN, COLOR_BLUE, COLOR_WHITE);

  /* Initialize "sprite" emulation area */
  emu_clear_sprite_area();
}

/* ---------------------------
   Hardware abstraction stubs
   --------------------------- */

/* NOTE: The following implementations are skeletons. Replace the POKEs /
   register addresses with correct values for the TED and the runtime
   you are using. Use your cc65 plus4/ted headers if they exist. */

/* Wait for vertical blank — on Plus/4 you must poll TED status or delay.
   Here we provide a small busy-wait placeholder. */
static void ted_hw_wait_vsync(void) {
  /* TODO: implement waiting on TED vblank bit or use a small delay if ok. */
  /* Simple busy loop as placeholder: */
  unsigned int i;
  for (i = 0; i < 3000; i++) { asm("nop"); }
}

static void ted_hw_set_text_color(unsigned char v) {
  /* TODO: map your 'v' to the Plus/4's text color register(s).
     On the C64 we used POKE(646,val) — on Plus/4 it may be different.
     If you want to set the foreground color for further cputs, use the
     runtime's conio color functions or write to the TED registers here. */
    textcolor(v);
}

static void ted_hw_set_charset(unsigned int addr) {
  /* TODO: implement mapping of character generator location.
     Many Plus/4 implementations require writing to a TED register or
     to a system page to change which RAM area holds char graphics. */
  /* Placeholder: no-op, or POKE to an imaginary TED register: */
  /* POKE(TED_CHARSET_REG, addr >> 8); */
    TED.misc &= 0xFB;
    TED.misc |= 0x04;
    TED.char_addr = 0xD0;

    // TED.char_addr = addr >> 8;
}

static void ted_hw_set_screen(unsigned int addr) {
  /* TODO: tell the TED / kernal / runtime to use 'addr' as screen memory */
  /* Example placeholder: */
  (void)addr;
}

static void ted_hw_set_bank(unsigned int addr) {
  /* On the C64 you used CIA2.pra bank manipulation — on Plus/4 banks work differently.
     Put required bank switching code here. */
  (void)addr;
}

static void ted_hw_set_border_bg_colors(unsigned char border, unsigned char bg0, unsigned char bg1, unsigned char bg2) {
  /* On C64 border is 0xD020 etc. On TED this may be different — set the
     border and background bytes here. Use POKE to appropriate registers. */
  /* Placeholder: use same addresses as C64 (may be wrong on Plus/4) */

    TED.bordercolor = border;
    TED.color1 = bg0;
    TED.color2 = bg1;
    TED.color3 = bg2;
}

static void ted_hw_write_color_ram(unsigned char *src, unsigned int len) {
  /* Color RAM mapping on Plus/4 differs. If COLOR_RAM is a defined symbol
     in your environment, write into it. We'll fallback to memcopy into
     a presumed COLOR_RAM base. */
  unsigned int i;
  for (i = 0; i < len; ++i) {
    /* adjust index mapping if needed */
    POKE((unsigned)COLOR_RAM + i, src ? src[i] : 8);
  }
}

static void ted_hw_disable_shift_charset(void) {
  /* On the C64 POKE(0x291, 0x80) disables shift->charset swapping.
     On Plus/4 this behavior is different; leave as placeholder. */
  POKE(0x291, 0x80); /* may be harmless; adjust if needed */
}

static void ted_hw_enable_shift_charset(void) {
  POKE(0x291, 0x00); /* placeholder */
}

/* ---------------------------
   Sprite emulation functions
   --------------------------- */

static void emu_clear_sprite_area(void) {
  /* Clear the emulated sprite memory area (we use SPRITE_LOC as scratch area) */
  memset((char*)SPRITE_LOC, 0, 256);
}

static void emu_draw_line_chars(unsigned char x, unsigned char y, unsigned char w) {
  unsigned char i;
  /* Convert a horizontal 'line' into repeated special characters that we
     write into DBLBUF_LOC at the target location. This is a very simple
     emulation; you can expand to modify charset patterns for finer control. */
  unsigned short base = (unsigned short)SCR + y * 40 + x;
  for (i = 0; i < w && (x + i) < 40; ++i) {
    POKE(base + i, '-');   /* ASCII dash variant — you may prefer custom char */
    POKE((unsigned)SCREEN_LOC + y * 40 + x + i, '-');
  }
}

static void emu_hide_line_chars(void) {
  /* Clear line emulation area: simply clear entire DBLBUF_LOC region used */
  /* For simplicity we zero the small scratch area; adapt as needed. */
  memset((char*)SCREEN_LOC, 0x20, 40 * 25); /* fill with spaces — adjust as needed */
}

#endif /* __PLUS4__ */
