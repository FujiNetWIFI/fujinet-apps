/**
 * adam-super-game-score-board
 *
 * @author Thom Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl 3.0 - see LICENSE.md
 */

#include <eos.h>
#include <string.h>
#include "dragonslair.h"

#define DRAGONSLAIR_BLOCK_NUM 16

extern unsigned char blockdev;

DragonslairScores dragonslairScores;

void dragonslair_init(void)
{
}

void dragonslair_read(void)
{
  eos_read_block(blockdev,DRAGONSLAIR_BLOCK_NUM,(unsigned char *)dragonslairScores);
}

void dragonslair_write(void)
{
  eos_write_block(blockdev,DRAGONSLAIR_BLOCK_NUM,(unsigned char *)dragonslairScores);
}
