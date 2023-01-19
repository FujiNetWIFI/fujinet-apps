/**
 * adam-super-game-score-board
 *
 * @author Thom Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl 3.0 - see LICENSE.md
 */

#include <eos.h>
#include <string.h>
#include "dkjr.h"

#define DKJR_BLOCK_NUM 169

extern unsigned char blockdev;

DkjrScores dkjrScores;

void dkjr_init(void)
{
  memcpy(dkjrScores.bottom,dkjr_bottom,sizeof(dkjr_bottom));
}

void dkjr_read(void)
{
  eos_read_block(blockdev,DKJR_BLOCK_NUM,(unsigned char *)dkjrScores);
}

void dkjr_write(void)
{
  eos_write_block(blockdev,DKJR_BLOCK_NUM,(unsigned char *)dkjrScores);
}
