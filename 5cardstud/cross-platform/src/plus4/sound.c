#ifdef __PLUS4__
/*
  Platform specific sound functions for Commodore Plus/4 (TED chip)
*/

#include <stdint.h>
#include <plus4.h>
#include "../misc.h"

void waitvsync();

// delay of played note
static unsigned char delay;

#define TED_FREQ1L   (*(volatile uint8_t*)0xFF0E)
#define TED_FREQ1H   (*(volatile uint8_t*)0xFF0F)
#define TED_FREQ2L   (*(volatile uint8_t*)0xFF10)
#define TED_FREQ2H   (*(volatile uint8_t*)0xFF11)
#define TED_CTRL     (*(volatile uint8_t*)0xFF12)
#define TED_NOISE    (*(volatile uint8_t*)0xFF13)

// --- Conversion helpers ---
static unsigned sidToTedTone(unsigned sidFreq) {
    unsigned long f_sid;
    unsigned long tedWord;

    if (sidFreq == 0) return 0x0FFF;

    f_sid = ((unsigned long)sidFreq * 985248UL) >> 16;
    if (f_sid == 0) return 0x0FFF;

    tedWord = (886724UL / (16UL * f_sid)) - 1;
    if (tedWord > 0x0FFF) tedWord = 0x0FFF;

    return (unsigned)tedWord;
}

static uint8_t sidToTedNoise(unsigned sidFreq) {
    unsigned long f_sid;
    unsigned long tedWord;

    if (sidFreq == 0) return 0xFF;

    f_sid = ((unsigned long)sidFreq * 985248UL) >> 16;
    if (f_sid == 0) return 0xFF;

    tedWord = (886724UL / (16UL * f_sid)) - 1;
    if (tedWord > 0xFF) tedWord = 0xFF;

    return (uint8_t)tedWord;
}

// --- Sound primitives ---
void playNote(unsigned f) {
    unsigned tf;

    tf = sidToTedTone(f);

    TED_FREQ1L = (uint8_t)(tf & 0xFF);
    TED_FREQ1H = (uint8_t)(tf >> 8);

    TED_CTRL = 0x80 | 0x10 | 8;  /* master, chan1 on, volume=8 */
    pause(delay);
    TED_CTRL = 0x80;             /* master on, channels off */
}

void playNoise(unsigned f) {
    uint8_t nf;

    nf = sidToTedNoise(f);
    TED_NOISE = nf;

    TED_CTRL = 0x80 | 0x20 | 8;  /* master, chan2 noise, vol=8 */
    pause(delay);
    TED_CTRL = 0x80;
}

void initSound() {
    TED_FREQ1L = 0;
    TED_FREQ1H = 0;
    TED_FREQ2L = 0;
    TED_FREQ2H = 0;
    TED_NOISE  = 0;
    TED_CTRL   = 0x80; /* master on, volume 0 */
}

// --- Higher-level sound API ---
void soundJoinGame() {
    delay=10;
    playNote(0x1a9d);
    playNote(0x151f);
    delay=20;
    playNote(0x1fa6);
}

void soundMyTurn() {
    delay=10;
    playNote(0x191e);
    delay=20;
    playNote(0x191e);
}

void soundGameDone() {
    static unsigned note;

    delay=8;
    for(note=0x109d; note<0x1fa6; note+=0x350)
        playNote(note);
    delay=20;
    playNote(note);
}

void soundDealCard() {
    delay=10;
    playNoise(0x8000);
}

void soundTick() {
    delay=4;
    playNoise(0x0600);
}

void soundPlayerJoin() {
    delay=25;
    playNoise(0x0630);
    playNoise(0x0640);
    playNoise(0x0650);
}

void soundPlayerLeft() {
    delay=20;
    playNoise(0x0760);
    playNoise(0x0750);
}

void soundSelectMove() {
    delay=5;
    playNote(0x191e);
    playNote(0x1c32);
}

void soundCursor() {
    delay=3;
    playNote(0x161e);
}

void soundCursorInvalid() {
    delay=3;
    playNote(0x121e);
}

void soundTakeChip(uint16_t counter) {
    unsigned freq;

    delay=2;
    freq = 0x0800 + counter*40;
    playNote(freq);
    pause(3);
}

#endif /* __PLUS4__ */

