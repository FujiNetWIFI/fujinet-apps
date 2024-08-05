/*  pmode4game.h - PMODE 4 playing cards library for the Disk Basic.
   
    By Pierre Sarrazin <http://sarrazip.com/>.
    This file is in the public domain.
*/

#ifndef _H_pmode4game
#define _H_pmode4game

#if !defined(_COCO_BASIC_) && !defined(DRAGON) && !defined(OS9)
#error This program must be compiled for a CoCo Disk Basic or Dragon or OS-9 environment.
#endif

#include <coco.h>
#include <stdarg.h>
#include "8-bit-wide-font.h"  /* text font bitmaps */


#ifndef NDEBUG
#define assertf(cond, ...) do { if (!(cond)) { \
                        screen(0, 0); \
                        printf("***ASSERT FAILED (%s): %u: %s: ", __TIME__, __LINE__, #cond); \
                        printf(__VA_ARGS__); putchar('\n'); \
                        for (;;); } } while (0)
#else
#define assertf(cond, ...) ((void) 0)
#endif

#define WORDS_PER_SCREEN_ROW    16
#define BYTES_PER_SCREEN_ROW    32
#define TEXT_COLS_PER_ROW       32
#define TEXT_ROWS_PER_SCREEN    24
#define PIXEL_ROWS_PER_SCREEN   192
#define BYTES_PER_SCREEN        0x1800  // 256 * 192 / 8 == 6144 == 6 kB
#define PIXEL_ROWS_PER_GLYPH    7       // see 8-bit-wide-font.h
#define PIXEL_ROWS_PER_TEXT_ROW 8       // PIXEL_ROWS_PER_GLYPH plus one separator row
#define BYTES_PER_TEXT_ROW      256     // 32 bytes per row of pixels, 8 rows of pixels per text row

#ifdef OS9
#define optDelay(ticks) (OS9Timer_delay(ticks))  /* because delay() uses F$Sleep */
#else
#define optDelay(ticks) (delay(ticks))
#endif

extern byte *scrnBuffer;  // 1st PMODE 4 buffer
extern byte textPosX;  // 0..BYTES_PER_SCREEN_ROW-1
extern byte textPosY;  // 0..TEXT_ROWS_PER_SCREEN-1
extern byte emulatingPMode4OnCoCo3;
extern ConsoleOutHook oldCHROUT;


///////////////////////////////////////////////////////////////////////////////


// Concatenates one or more strings into an output buffer.
// out: Output buffer.
// max: Maximum number of characters to write to the output buffer,
//      including the terminating '\0'.
//
// CAUTION: The last argument MUST be a null pointer.
//
// If the input strings together contain too many characters,
// the concatenation stops at 'max'.
// The returned string always finishes with a terminating '\0'.
//
void concat(char *out, unsigned max, ...);


///////////////////////////////////////////////////////////////////////////////


// Returns the screen address at which drawChar() would put the glyph.
// This address points to the top of the 8x8 pixel grid that would be
// occupied by the glyph. See PIXEL_ROWS_PER_TEXT_ROW and BYTES_PER_SCREEN_ROW.
//
byte *getCurrentCharPos(void);


// Used by drawChar().
//
typedef void (*TextAreaScrollCallback)(void *userdata);


// Sets the callback to be invoked by drawChar() when the cursor reaches
// the bottom right of the text area.
// userdata: Passed to (*callback)() when it is invoked.
// The callback is allowed to call moveCursor().
//
void setTextAreaScrollCallback(TextAreaScrollCallback callback, void *userdata);


// Displays the given character at the current cursor position and advances
// the cursor.
// When this function has just written to the last screen position,
// it invokes the callback set by setTextAreaScrollCallback() if that function
// has last been called with a non-null callback pointer, otherwise, the screen
// is not scrolled and the cursor goes home instead.
// Pass '\n' to go to the beginning of the next text row.
// No other cursor-moving control character is supported.
//
void drawChar(unsigned char c);


// Displays the given character at the given byte and at the 6 or 7 bytes below on the screen.
// Cursor-moving control characters are not supported.
// dest: Address of the top byte of the area to be changed.
// drawBottomRow: If TRUE, draws an 8th row below the 7 rows defined by the font.
//
void drawCharAtAddressWithoutMovingCursor(unsigned char c, byte *dest, BOOL drawBottomRow);


// Cursor-moving control characters are not supported.
//
void drawCharWithoutMovingCursor(unsigned char c);


// enabled: If TRUE, each byte from the font will be inverted before it is written to the screen.
//
void enableInvertedColorText(BOOL enabled);


// Sets a byte to be XORed with each byte from the font before writing the result to the screen.
//
void setColorInversionMask(byte newColorInversionMask);


// x: 0..TEXT_COLS_PER_ROW-1.
// y: 0..TEXT_ROWS_PER_SCREEN-1.
// Does nothing if either is out of range.
//
void moveCursor(byte x, byte y);


// Moves the cursor left by the specified number of columns, without leaving
// the current row. If numColumns exceeds the current horizontal position,
// the cursor will be left at the leftmost position of the current row.
//
void moveCursorLeft(byte numColumns);


// Reverses the colors of the pixels in the rectangle occupied by the current
// text cursor position.
// Does not advance the cursor.
//
void invertCharAtCursor(void);


// Same as invertCharAtCursor(), but does not invert the row below the rows
// defined by the glyph.
//
void invertCharAtCursorExceptRowBelowGlyph(void);


// See waitForKeyWithAnimatedCursor() and readLineFromKeyboard().
//
typedef byte (*KeyboardPollFunction)(void *userdata);


// See waitForKeyWithAnimatedCursor() and readLineFromKeyboard().
// 
typedef word (*GetTimerFunction)(void *userdata);


// Animates a cursor at the current cursor position while calling (*poll)(userdata)
// until this call returns a non-zero value. This value is returned.
// For example, 'poll' can be a pointer to a function like the following, when using
// the BControl library:
//   byte pollKeyboard(void *) { return Keyboard_poll(&keyboard); }
// ticksPerCursorPeriod: Duration of the cursor animation period, in ticks.
//                       Pass 0 to show no cursor at all.
// getTimerValue: Must return a value that is incremented 60 times per second.
//                Receives userdata when called.
//                Ignored if ticksPerCursorPeriod is 0. 
//
byte waitForKeyWithAnimatedCursor(KeyboardPollFunction poll,
                                  void *userdata,
                                  word ticksPerCursorPeriod,
                                  GetTimerFunction getTimerValue);


// Animates a cursor at the current cursor position and lets the user type a line
// ended with pressing Enter. The backspace key is supported.
// poll, userdata, ticksPerCursorPeriod, getTimerValue: See waitForKeyWithAnimatedCursor().
// lineBuffer: Array of characters that will receive the NUL-terminated string typed by the user.
// bufferSize: Size of the lineBuffer array in bytes. Must include space a the NUL terminator.
// Returns TRUE if the user pressed Enter, or FALSE if the user pressed Break.
// If TRUE is returned, lineBuffer contains a NUL-terminated string.
//
BOOL readLineFromKeyboard(KeyboardPollFunction poll,
                          void *userdata,
                          word ticksPerCursorPeriod,
                          GetTimerFunction getTimerValue,
                          char *lineBuffer, size_t bufferSize);


// Called by PUTCHR, which is called by printf() et al.
// The character to print is in A.
// It is sent to drawChar() (the 32x24 screen) instead of
// the regular CoCo screen.
//
void hiResTextConsoleOutHook();


// row: Text line where to print (2nd argument to moveCursor()).
//      Must not be more than 'textScreenWidth' characters.
// Returns the text column where 'str' gets printed. 
//
byte printCentered(byte row, const char *str);


// Makes the PMODE 4 screen at the given address visible.
// If that mode is being emulated on a CoCo 3, the address
// must be an offset from physical address $70000.
//
// This function remembers the given address as the start of the screen,
// which affects other functions in this file when they determine a character's
// address in the screen buffer.
//
// Not supported under OS-9.
//
void setScreenBuffer(void *screen);


// Sets this library's internal graphics screen pointer to 'screen'.
// Does NOT change the graphics mode.
// Usable under OS-9.
//
void setScreenBuffer2(void *screen);


// Initializes CoCo support functions.
// Does the equivalent of PMODE 4:SCREEN 1,1.
// Does not assume the presence of ECB.
//
// _scrnBuffer: Address of the 6k graphics buffer. Must be divisible by 512.
//              Ignored under OS-9.
// _emulatePMode4OnCoCo3: Uses a CoCo 3 graphics mode to emulate PMODE 4
//                        and its artifact colors.
//
// Global variable scrnBuffer is initialized with _scrnBuffer, except under OS-9,
// where it is initialized with the screen buffer address given by the system.
// This affects other functions in this file when they determine a character's
// address in the screen buffer.
//
// Initializes the 32x24 software text screen's cursor at the home position.
// Remembers the initial text mode (32, 40, 80), on a CoCo 3.
// Initializes coco.h globals textScreenWidth and textScreenHeight.
// Redirects printf() so that it prints to the software text screen.
//
void openPMode4Game(void *_scrnBuffer, byte _emulatePMode4OnCoCo3);


// Releases resources allocated by openPMode4Game().
// Does not leave graphics mode.
// Under OS-9, the program should do this to leave graphics mode:
//   putstr("\x12\x0E", 2);  // End Graphics, Display Alpha (text mode)
// Under the native CoCo environment, the program should call screen(0, 0).
//
void closePMode4Game(void);


// Callback used by runPMode4GameDoubleBufferLoop().
// hiddenBufferIndex: 0..1.
// userdata: Cookie received by runPMode4GameDoubleBufferLoop() and passed as is to this callback.
//
typedef BOOL (*HiddenBufferUpdateFunction)(byte hiddenBufferIndex, void *userdata);


// Shows a PMODE 4 screen then runs a double-buffered loop that alternates between
// the two designated screen buffers.
// graphBuffers: Addresses of two PMODE 4 buffers. Must be divisible by 512.
// updateFunc: Function to be called upon a double buffer flip.
//             It is called with the index (0 or 1) of the buffer that has just
//             become hidden. It is also called with 'userdata'.
// numIRQsBeforeScreenFlipPtr: Pointer to a byte that gets incremented periodically, typically
//                             by the IRQ service routine.
//                             When it reaches numIRQsPerBufferFlip, the buffers get flipped.
// numIRQsPerBufferFlip: Number of IRQ interrupts before a buffer flip.
//                       On a CoCo, 3 means a flip every 20th of a second.
// userdata: Passed as is to updateFunc.
//
void runPMode4GameDoubleBufferLoop(byte *graphBuffers[2],
                                   HiddenBufferUpdateFunction updateFunc,
                                   byte *numIRQsBeforeScreenFlipPtr,
                                   byte numFramesPerBufferFlip,
                                   void *userdata);


#endif  /* _H_pmode4game */
