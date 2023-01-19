/**
 * adam-super-game-score-board
 *
 * @author Thom Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl, 3.0 - see LICENSE.md
 */

#ifndef DRAGONSLAIR_H
#define DRAGONSLAIR_H

typedef struct _dragonslair_scores
{
  char scores[8][7];
  char names[8][12];
  char sk[8][2];
  char sc[8][2];
  char rsvd1[12]; // Default name, do not touch
  char bottom[828]; // Bottom of block.
} DragonslairScores;

void dragonslair_init(void);
void dragonslair_read(void);
void dragonslair_write(void);

#endif /* DRAGONSLAIR_H */
