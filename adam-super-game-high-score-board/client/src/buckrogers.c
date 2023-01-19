/**
 * adam-super-game-score-board
 *
 * @author Thom Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl 3.0 - see LICENSE.md
 */

#include <eos.h>
#include <string.h>
#include "buckrogers.h"

#define BUCKROGERS_BLOCK_NUM 141

extern unsigned char blockdev;

BuckrogersScores buckrogersScores;

void buckrogers_read(void)
{
  eos_read_block(blockdev,BUCKROGERS_BLOCK_NUM,(unsigned char *)buckrogersScores);
}

void buckrogers_write(void)
{
  eos_write_block(blockdev,BUCKROGERS_BLOCK_NUM,(unsigned char *)buckrogersScores);
}
