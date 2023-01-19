/**
 * adam-super-game-score-board
 *
 * @author Thom Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl 3.0 - see LICENSE.md
 */

#include <eos.h>
#include <string.h>
#include "dkong.h"

#define DKONG_BLOCK_NUM 187 

extern unsigned char blockdev;

DkongScores dkongScores;

void dkong_init(void)
{
}

void dkong_read(void)
{
  eos_read_block(blockdev,DKONG_BLOCK_NUM,(unsigned char *)dkongScores);
}

void dkong_write(void)
{
  eos_write_block(blockdev,DKONG_BLOCK_NUM,(unsigned char *)dkongScores);
}
