#ifdef __ATARI__
/*
  Platform specific sound functions
*/

#include <atari.h>
#include <string.h>
#include <peekpoke.h>

// Set to control delay of played note
static unsigned char delay;

void initSound() {
  // Silence SIO noise
  OS.soundr = 0;
}


void playNote(unsigned f) {
  
  //  static unsigned char i;
  //  SID.v1.freq=f;
  //  SID.v1.ctrl = 0x11;
  //  for(i=delay;--i;)
  //   waitvsync();
  //  SID.v1.ctrl = 0x10;
}

void playNoise(unsigned f) {
  //  static unsigned char i;
  //  SID.v2.freq=f;
  //  SID.v2.ctrl = 0x81;
  //  for(i=delay;--i;)
  //   waitvsync();
  //  SID.v2.ctrl = 0x80;
}



void soundJoinGame() {
 
}

void soundMyTurn() {
 }

void soundGameDone() {
}

void soundRollDice() {
}

void soundPlayerJoin() {
}

void soundPlayerLeft() {
}

void soundSelectMove() {
}

void soundCursor() {
}

void soundCursorInvalid() {
}

void soundCursorSelect() {
}

void soundCursorDeselect() {

}

void soundTick() {
  _sound(0, 200,8,3);
  waitvsync();
  _sound(0,0,0,0);
}


#endif /* __ATARI__ */