/*  cardgame.h - PMODE 4 playing cards library for Disk Basic.
   
    By Pierre Sarrazin <http://sarrazip.com/>.
    This file is in the public domain.
*/

#ifndef _H_cardgame
#define _H_cardgame

#include "pmode4game.h"
#include "compiled-cards.h"   /* card bitmaps */

#ifdef OS9
#include "OS9Timer.h"
#endif


#define NUMCARDS                54
#define BAD_CARD_SUIT           0xFF
#define BAD_CARD_VALUE          0
#define BAD_INDEX               0xFF

#define ROWS_ABOVE_SUIT 2
#define ROWS_BELOW_SUIT 6
#define ROWS_PER_CARD_TOP 5
#define ROWS_PER_CARD_BOTTOM ROWS_PER_CARD_TOP 
#define BYTES_PER_CARD_WIDTH 4
#define ROWS_PER_CARD_VALUE 10
#define ROWS_PER_SUIT 7

#define SUIT_HEARTS   0
#define SUIT_SPADES   1
#define SUIT_DIAMONDS 2
#define SUIT_CLUBS    3
#define SUIT_JOKER    4
#define SUIT_NONE     255


///////////////////////////////////////////////////////////////////////////////


// byteOffset: Offset in bytes from the left of the screen (must be < BYTES_PER_SCREEN_ROW).
// pixelRow: Vertical position (in pixels) of the row at which to start drawing (downwards).
//           Must be < PIXEL_ROWS_PER_SCREEN.
// numRows: Must be > 0.
// rowRepetitions: Must be > 0.
// Returns the address of the top left byte of the region that was modified.
// Uses scrnBuffer as the start of the PMODE 4 screen buffer.
//
byte *drawCompiledPixMap(byte byteOffset, byte pixelRow, const word *wordArray, byte numRows, byte rowRepetitions);


// cardValue: 1..13 (1 = ace).
// cardSuit: SUIT_* (0..3).
// Except for jokers: cardValue must be 1 (red) or 2 (black) and cardSuit must be SUIT_JOKER.
// byteColumn: 0..28 (maps to pixel columns 0, 8, 16, ..., 224).
// pixelRow: 0..150.
//
// The number of pixels rows occupied by the card is given by getNumPixelRowsPerCard().
// A card is 4 bytes wide, i.e., 32 pixels.
//
void drawCompiledCard(byte cardValue, byte cardSuit, byte byteColumn, byte pixelRow);


// Height of a card drawn by drawCompiledCard().
// A card is 4 bytes wide, i.e., 32 pixels.
//
byte getNumPixelRowsPerCard();


void eraseCard(byte byteColumn, byte pixelRow);


// rowInPixels: Offset in pixels down the top of the screen.
// colInBytes: Horizontal position of the card, in bytes (0..28).
//
void drawFaceDownCard(byte rowInPixels, byte colInBytes);


byte getCardType(byte card);


byte getCardValue(byte card);


const char *getSuitName(byte suit);


void shuffleDeck(byte deck[], byte numCards);


// Removes a card from the end of the designated deck, which
// initially contains a number of cards given by *pNumCards,
// decrements this number, then returns the removed card.
// (The "top" of a deck is at the highest valid index,
// while the "bottom" is at index 0.)
//
byte pickCardFromDeck(byte deck[], byte *pNumCards);


// col: Byte offset in targeted row.
// row: Pixel row where to draw top of suit.
// suit: Index into suits[].
//
void drawSuit(byte col, byte row, byte suit);


// Calls openPMode4Game().
//
void openCardGame(void *_scrnBuffer, byte _emulatePMode4OnCoCo3);


// Calls closePMode4Game().
//
void closeCardGame(void);


enum
{
    BEEP_MIN_VOL = 0,
    BEEP_MAX_VOL = 63,
};


// Must be called before using the functions that follow.
//
void initSound();


// Returns the number of ticks to pass to beep() to have it play a
// square wave of the given frequency (in hertz).
//
word getTicksForFrequency(word hertz);


// initSound() MUST have been called before calling this function.
// duration: Must be positive (max 255).
//           Should be >= 5 for usable sound.
// ticks: Length of the half period of the square wave.
//        See getTicksForFrequency().
//        Larger number means lower sound.
//        Should be <= 4000 for usable sound. Should not be 0.
// volume: 0..BEEP_MAX_VOL.
//
void beep(byte duration, word ticks, byte volume);


// duration: Must be positive (max 255).
//           Should be >= 5 for usable sound.
// spacing: 0..65535: length of each random sample send to the speaker.
// volume: 0..BEEP_MAX_VOL.
//
void whiteNoise(word duration, word spacing, byte volume);


#endif  /* _H_cardgame */
