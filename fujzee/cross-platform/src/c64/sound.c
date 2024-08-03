#ifdef __C64__
/*
  Platform specific sound functions
*/

#include <c64.h>
#include <string.h>

void waitvsync();

// Set to control delay of played note
static unsigned char delay;

void playNote(unsigned f) {
   static unsigned char i;
   SID.v1.freq=f;
   SID.v1.ctrl = 0x11;
   for(i=delay;--i;)
    waitvsync();
   SID.v1.ctrl = 0x10;
}

void playNoise(unsigned f) {
   static unsigned char i;
   SID.v2.freq=f;
   SID.v2.ctrl = 0x81;
   for(i=delay;--i;)
    waitvsync();
   SID.v2.ctrl = 0x80;
}

void initSound() {
  static unsigned char j,i=15;
  memset(&SID,0,25);

  // Ramp up volume to avoid a harsh pop
  while(--i) {
    SID.amp++;
    j=100;while(--j);
  }

  // Set default values for voices
  SID.v1.ad=0x09;
  SID.v1.sr=0x00;
  SID.v2.sr=0x00;
}

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
  for(note=0x109d;note<0x1fa6;note+=0x350)
    playNote(note);
  delay=20;
  playNote(note);
}

void soundDealCard() {
  SID.v2.ad=0x22;
  delay=10;
  playNoise(0x8000);
}

void soundTick() {
  SID.v2.ad=0x11;
  delay=4;
  playNoise(0x600);
}

void soundPlayerJoin() {
  SID.v2.ad=0x56;
  delay=25;
  playNoise(0x630);
  playNoise(0x640);
  playNoise(0x650);
}

void soundPlayerLeft() {
  SID.v2.ad=0x56;
  delay=20;
  playNoise(0x760);
  playNoise(0x750);
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

#endif /* __C64__ */