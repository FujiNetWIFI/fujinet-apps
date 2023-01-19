/**
 * adam-super-game-score-board
 *
 * @author Thom Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl, 3.0 - see LICENSE.md
 */

#ifndef BUCKROGERS_H
#define BUCKROGERS_H

typedef struct _buckrogers_scores
{
  char scores[8][7];
  char names[8][12];
  char sk[8][2];
  char sc[8][2];
  char rsvd1[12]; // Default name, do not touch
  char bottom[828]; // Bottom of block.
} BuckrogersScores;

void buckrogers_init(void);
void buckrogers_read(void);
void buckrogers_write(void);

#endif /* BUCKROGERS_H */
