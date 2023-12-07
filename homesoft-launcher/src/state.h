/**
 * @brief   Homesoft Launcher
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details
 */

#ifndef STATE_H
#define STATE_H

typedef enum _state
  {
    INIT = 0,
    SELECT,
    SEARCH,
    MOUNT,
    BOOT
  }
  State;
#endif /* STATE_H */
