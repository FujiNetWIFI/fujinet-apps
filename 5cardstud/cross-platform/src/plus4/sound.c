#ifdef __PLUS4__
/*
  Platform specific sound functions for Commodore Plus/4 (TED chip)
*/

#include <plus4.h>
#include <string.h>
#include <stdint.h>
#include "../misc.h"
#include "../platform-specific/sound.h"
#include <stdio.h>
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
    unsigned dd;
    // Turn on the square
    TED.snd_ctrl = v | 0x10; // Enable channel 1, with volume v
    TED.snd1_freq_lo = fl;   // Set lo bits
    TED.misc &= 0xFC;        // zap high bits, preserving misc register
    TED.misc |= fh;          // set high bits.
    while (v>0) 
    {
        dd=d;
        // Wait # of vblanks.
        while (dd--)
            waitvsync();

        // Reduce volume
        --v;
        TED.snd_ctrl = v | 0x10; 
    }
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
    unsigned dd;

    // Turn on the square
    TED.snd_ctrl = v | 0x40; // Enable channel 2, with noise; with volume v
    TED.snd2_freq_lo = fl;   // Set lo bits
    TED.snd2_freq_hi = fh;   // Set hi bits

    while (v>1) 
    {
        dd=d;
        // Wait # of vblanks.
        while (dd--)
            waitvsync();

        // Reduce volume
        --v;
        TED.snd_ctrl = v | 0x40; 
    }

    // Turn off the square
    TED.snd_ctrl = 0x00;     // Kill the sound
}

void initSound()
{
    // Not needed because it's taken care of in playNote/playNoise
}

void soundJoinGame()
{
    playNote(392,2,6);
    playNote(330,2,6);
    playNote(440,4,6);
}

void soundMyTurn()
{
   
    playNote(440,3,5);
    playNote(440,3,5);
}

void soundGameDone()
{
    unsigned short slide;

    playNote(220,3,6);
    playNote(440,2,6);
    playNote(880,4,6);
}

void soundDealCard()
{
    playNoise(96,1,4);
    pause(4);
}

void soundTick()
{
    playNoise(24,1,5);
}

void soundPlayerJoin()
{
    playNoise(120,1,4);
    playNoise(100,1,4);
    pause(20);
    playNoise(120,1,5);
    playNoise(100,1,5);
    pause(20);
    playNoise(120,1,7);
    playNoise(100,1,7);
    
}

void soundPlayerLeft()
{
    playNoise(120,1,7);
    playNoise(100,1,7);
    pause(20);
    playNoise(120,2,5);
    playNoise(100,1,5);
    pause(20);
    playNoise(120,2,3);
    playNoise(100,2,3);
}

void soundSelectMove()
{
    playNote(350,1,6);
    playNote(420,2,6);
}

void soundCursor()
{
    playNote(600,1,6);
}

void soundCursorInvalid()
{
    playNote(300,1,6);
}

void soundTakeChip(uint16_t counter)
{
    playNote(150+counter*5,1,4);
    pause(1);
}

#endif /* __PLUS4__ */
