/**
 * adam-super-game-score-board
 *
 * @author Thom Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl, 3.0 - see LICENSE.md
 */

#ifndef DKJR_H
#define DKJR_H

typedef struct _dkjr_scores
{
  char scores[8][7];
  char names[8][12];
  char sk[8][2];
  char sc[8][2];
  char rsvd1[12]; // Default name, do not touch
  char bottom[828]; // Bottom of block.
} DkjrScores;

void dkjr_init(void);
void dkjr_read(void);
void dkjr_write(void);

#endif /* DKJR_H */
