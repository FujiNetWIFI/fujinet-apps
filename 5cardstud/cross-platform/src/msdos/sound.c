#include <dos.h>
#include <stdint.h>

// PC Speaker ports and values
#define PIT_CONTROL_PORT 0x43
#define PIT_CHANNEL2_PORT 0x42
#define SPEAKER_CONTROL_PORT 0x61

// PIT frequency
#define PIT_FREQUENCY 1193180UL

/* Because I don't want to drag all of conio into this project! */
_WCIRTLINK extern unsigned inp(unsigned __port);
_WCIRTLINK extern unsigned outp(unsigned __port, unsigned __value);

/**
 * @brief Beep the speaker for the specified # of VBLANK frames
 * @param frequency Frequency in Hz
 * @param frames # of vertical blank intervals (approx 16.67ms per interval)
 * @param wait # of vertical blank intervals to wait.
 */
void beep(unsigned int frequency, unsigned int frames, unsigned int wait) {
    unsigned int divisor;
    unsigned char tmp;

    // Calculate the divisor for the given frequency
    divisor = PIT_FREQUENCY / frequency;

    // Set the PIT to mode 3 (square wave) on channel 2
    outp(PIT_CONTROL_PORT, 0xB6); // 1011 0110

    // Send frequency divisor to channel 2 (low byte, then high byte)
    outp(PIT_CHANNEL2_PORT, divisor & 0xFF);        // Low byte
    outp(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF); // High byte

    // Turn on the speaker (enable bit 0 and 1)
    tmp = inp(SPEAKER_CONTROL_PORT);
    outp(SPEAKER_CONTROL_PORT, tmp | 0x03);

    // Delay for # of frames
    while (frames--)
    {
        // Wait until vblank starts
        while(!(inp(0x3DA) & 0x08));
        // Wait until vblank stops
	while(inp(0x3DA) & 0x08);
    }

    // Turn off the speaker (clear bit 0, keep bit 1 for PIT gate)
    tmp = inp(SPEAKER_CONTROL_PORT);
    outp(SPEAKER_CONTROL_PORT, tmp & ~0x03);

    // Wait for # of frames
    while (wait--)
    {
        // Wait until vblank starts
        while(!(inp(0x3DA) & 0x08));
        // Wait until vblank stops
	while(inp(0x3DA) & 0x08);
    }
}

void initSound()
{
    // Not really used.
}

/**
 * to Eric: None of these values are right. ;)
 */
void soundJoinGame()
{
    beep(340,40,115);
    beep(440,40,40);
    beep(280,40,0);
}

void soundMyTurn()
{
    beep(340,40,50);
    beep(340,40,0);
}

void soundGameDone()
{
    beep(830,20,20);
    beep(790,50,30);
    beep(650,20,20);
    beep(610,40,50);
}

void soundDealCard()
{
    beep(150,1,5);
}

void soundTick()
{
    beep(800,2,0);
}

void soundPlayerJoin()
{
    int i=255;
    for (i=255;i>=235;i-=10)
        beep(i*10,5,255);
}

void soundPlayerLeft()
{
    int i=255;
    for (i=215;i<255;i+=10)
        beep(i*10,5,255);
}

void soundSelectMove()
{
    beep(350,40,20);
    beep(300,40,0);
}

void soundCursor()
{
    beep(450,10,0);
}

void soundCursorInvalid()
{
    beep(1180,5,0);
}

void soundTakeChip(uint16_t counter)
{
    beep(1180-counter*3,5,60);
}
