/**
 * adam-super-game-score-board
 *
 * @author Thom Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl 3.0 - see LICENSE.md
 */

#include <msx.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dragonslair.h"

unsigned char blockdev;

extern DragonslairScores dragonslairScores;

void main(void)
{
  char score[8];
  char name[13];
  char sk[3];
  char sc[3];
  
  blockdev=0x05;

  dragonslair_init();
  
  cprintf("INSERT DRAGONSLAIR INTO TAPE 2.\n");
  cgetc();

  cprintf("READING HI-SCORE BLOCK.\n");
  dragonslair_read();

  cprintf("PRINTING HI-SCORE BLOCK\n");

  for (int i=0;i<8;i++)
    {
      memset(score,0,sizeof(score));
      memset(name,0,sizeof(name));
      memset(sk,0,sizeof(sk));
      memset(sc,0,sizeof(sc));

      memcpy(&score,&dragonslairScores.scores[i],7);
      memcpy(&name,&dragonslairScores.names[i],12);
      memcpy(&sk,&dragonslairScores.sk[i],2);
      memcpy(&sc,&dragonslairScores.sc[i],2);

      printf("%7s %12s %2s %2s\n",score,name,sk,sc);
    }
}
