#ifdef __ATARI__
/*
  Platform specific sound functions
*/

#include <atari.h>
#include <string.h>
#include <peekpoke.h>
#include <stdlib.h>
#include "../misc.h"

#include "../platform-specific/sound.h"

// Set to control delay of played note
static unsigned char delay;

void initSound() {
  // Silence SIO noise
  OS.soundr = 0;
  disableKeySounds();
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
  for(j=0;j<2;j++) {
    _sound(0,80,10,5);
    pause(2);
    for (i=7;i<255;i--) {
      _sound(0,80,10,i);
      waitvsync();
    }
    waitvsync();
  }
 }

void soundGameDone() {
  _sound(0,150,10,8);pause(3);
  _sound(0,140,10,8);pause(4);
  _sound(0,135,10,8);pause(3);
  _sound(0,132,10,8);pause(4);
  soundStop();
}

void soundRollDice() {
  _sound(0, 150+ (rand() % 20)*5,8,7);
}

void soundPlayerJoin() {
}

void soundPlayerLeft() {
}

void soundCursor() {
   _sound(0,100,10,5);
   pause(2);
   soundStop();
}

void soundCursorScore() {
  _sound(0,90,10,5);
  pause(1);
  soundStop();
}

void soundKeep() {
  j=0;
  for(i=200;i>150;i-=10) {
    _sound(0,i,10,2+j++);
    waitvsync();
  }
  soundStop();
}

void soundRelease() {
 for(i=6;i<255;i--) {
    _sound(0,200,10,i);
    waitvsync();
  }
}

void soundTick() {
  _sound(0, 200,8,7);
  waitvsync();
  _sound(0,0,0,0);
}

void soundStop() {
  _sound(0,0,0,0);
}

void disableKeySounds() {
  OS.noclik = 255;
}

void enableKeySounds() {
  OS.noclik = 0;
}

void soundScore() {
  j=0;
  for(i=80;i>50;i-=10) {
    _sound(0,i,10,2+j++);
    waitvsync();
  }
  soundStop();
}

#endif /* __ATARI__ */