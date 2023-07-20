/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Message line routines
 */

extern const char *msg_source_move;
extern const char *msg_cant_do_that;
extern const char *msg_destination_move;

/**
 * @brief place s onto message bar
 * @param s String to display in message bar (0-20 chars)
 */
void msg(const char *s);
