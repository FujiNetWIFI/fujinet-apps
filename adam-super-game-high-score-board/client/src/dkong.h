/**
 * adam-super-game-score-board
 *
 * @author Thom Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl, 3.0 - see LICENSE.md
 */

#ifndef DKONG_H
#define DKONG_H

typedef struct _dkong_scores
{
  char scores[8][7];
  char names[8][12];
  char sk[8][2];
  char sc[8][2];
  char rsvd1[12]; // Default name, do not touch
  char bottom[828]; // Bottom of block.
} DkongScores;

void dkong_init(void);
void dkong_read(void);
void dkong_write(void);

#endif /* DKONG_H */
