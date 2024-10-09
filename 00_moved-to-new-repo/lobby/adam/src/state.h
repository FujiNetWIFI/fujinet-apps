/**
 * @brief FujiNet Lobby for Adam
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef STATE_H
#define STATE_H

typedef enum _state
  {
    INIT,
    USERNAME,
    CONNECT,
    FETCH,
    DISPLAY,
    SELECT,
    SHOUT,
    CHAT,
    HALT
  } State;

#endif /* STATE_H */
