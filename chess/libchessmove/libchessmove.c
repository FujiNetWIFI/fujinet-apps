/*
  libchessmove.c - Valid chess move library for cc65

  2023-07-14 - 2023-07-14
*/

#include "libchessmove.h"

/*
  valid() tests whether the move being attempted is valid.

  @param piece-t * board - an array that represents an 8x8 chess board (8 rows of 8 columns)
  FIXME: other game state we must keep
  @param int old_pos - old position on the board (0-63) FIXME: Just use char (unsigned byte)?
  @param int new_pos - new position on the board (0-63) FIXME: Just use char (unsigned byte)?
  @return char whether the move is valid, invalid, or an error
*/

char valid(piece_t * board, int old_pos, int new_pos) {
  if (old_pos > 63 || new_pos > 63 || old_pos < 0 || new_pos < 0)
    return CHESSMOVE_ERROR;

  /* Cannot move FROM an empty space */
  if (board[old_pos] == CHESSPIECE_EMPTY)
    return CHESSMOVE_INVALID;

  /* Cannot attack your own pieces */
  if (piece_color(board[old_pos]) == piece_color(board[new_pos]))
    return CHESSMOVE_INVALID;

  return CHESSMOVE_VALID;
}

