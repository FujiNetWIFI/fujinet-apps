/**
 * adam-super-game-score-board
 *
 * @author Thom Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl 3.0 - see LICENSE.md
 */

#include <eos.h>
#include <string.h>
#include "zaxxon.h"

#define ZAXXON_BLOCK_NUM 7

extern unsigned char blockdev;

ZaxxonScores zaxxonScores;

void zaxxon_init(void)
{
}

void zaxxon_read(void)
{
  eos_read_block(blockdev,ZAXXON_BLOCK_NUM,(unsigned char *)zaxxonScores);
}

void zaxxon_write(void)
{
  eos_write_block(blockdev,ZAXXON_BLOCK_NUM,(unsigned char *)zaxxonScores);
}
