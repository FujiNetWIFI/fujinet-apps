#ifdef __PLUS4__
/*
  Platform specific sound functions for Commodore Plus/4 (TED chip)
*/

#include <plus4.h>
#include <string.h>
#include <stdint.h>
#include "../misc.h"
#include "../platform-specific/sound.h"

/**
 * @brief play note through voice 1 (square)
 * @param f Frequency in Hz
 * @param d delay (gate) time in vertical blank intervals
 * @param v Volume (0-15)
 */
void playNote(unsigned f, unsigned d, unsigned char v)
{
    unsigned short fd = 1024 - (111861 / f);
    unsigned char fl = fd & 0xFF; // Frequency LO
    unsigned char fh = (fd & 0x0300) >> 8; // Frequency HI

    // Turn on the square
    TED.snd_ctrl = v | 0x10; // Enable channel 1, with volume v
    TED.snd1_freq_lo = fl;   // Set lo bits
    TED.misc &= 0xFC;        // zap high bits, preserving misc register
    TED.misc |= fh;          // set high bits.

    // Wait # of vblanks.
    while (d--)
        waitvsync();

    // Turn off the square
    TED.snd_ctrl = 0x00;     // Kill the sound
}

/**
 * @brief play noise through voice 2 (raucous squawking noise)
 * @param f Frequency in Hz
 * @param d delay (gate) time in vertical blank intervals
 * @param v Volume (0-15)
 */
void playNoise(unsigned f, unsigned d, unsigned char v)
{
    unsigned short fd = 1024 - (111861 / f);
    unsigned char fl = fd & 0xFF; // Frequency LO
    unsigned char fh = (fd & 0x0300) >> 8; // Frequency HI

    // Turn on the square
    TED.snd_ctrl = v | 0x40; // Enable channel 2, with noise; with volume v
    TED.snd2_freq_lo = fl;   // Set lo bits
    TED.snd2_freq_hi = fh;   // Set hi bits

    // Wait # of vblanks.
    while (d--)
        waitvsync();

    // Turn off the square
    TED.snd_ctrl = 0x00;     // Kill the sound
}

void initSound()
{
    // Not needed because it's taken care of in playNote/playNoise
}

void soundJoinGame()
{
    playNote(392,10,9);
    playNote(330,10,9);
    playNote(440,20,9);
}

void soundMyTurn()
{
    playNote(440,10,9);
    pause(8);
    playNote(440,20,9);
}

void soundGameDone()
{
    unsigned short slide;

    playNote(220,8,9);
    playNote(440,16,9);
    playNote(880,16,9);
}

void soundDealCard()
{
    playNoise(1000,5,9);
}

void soundTick()
{
    playNoise(200,4,9);
}

void soundPlayerJoin()
{
    playNoise(400,25,9);
    playNoise(420,25,9);
    playNoise(440,25,9);
}

void soundPlayerLeft()
{
    playNoise(600,20,9);
    playNoise(575,20,9);
}

void soundSelectMove()
{
    playNote(350,5,9);
    playNote(420,5,9);
}

void soundCursor()
{
    playNote(600,3,9);
}

void soundCursorInvalid()
{
    playNote(300,3,9);
}

void soundTakeChip(uint16_t counter)
{
    playNote(150+counter*40,2,9);
    pause(3);
}

#endif /* __PLUS4__ */
