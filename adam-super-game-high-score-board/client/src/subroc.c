/**
 * adam-super-game-score-board
 *
 * @author Thom Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl 3.0 - see LICENSE.md
 */

#include <eos.h>
#include <string.h>
#include "subroc.h"

#define SUBROC_BLOCK_NUM 43

extern unsigned char blockdev;

SubrocScores subrocScores;

void subroc_init(void)
{
}

void subroc_read(void)
{
  eos_read_block(blockdev,SUBROC_BLOCK_NUM,(unsigned char *)subrocScores);
}

void subroc_write(void)
{
  eos_write_block(blockdev,SUBROC_BLOCK_NUM,(unsigned char *)subrocScores);
}
