/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   State typedefs
 */

#ifndef STATE_H
#define STATE_H

typedef enum _main_state
  {
    SPLASH,
    MAIN_MENU,
    DIRECTORY,
    DIRECTORY_MAKE,
    DIRECTORY_DEL,
    DIRECTORY_GET,
    DIRECTORY_PUT,
    GET_URL,
    PUT_URL,
    NC,
    DONE
  } MainState;

typedef enum _directoryState
  {
    SELECT_DEVICE,
    SHOW_DIRECTORY,
    SELECT_FILE,
    
  } DirectoryState;

extern MainState mainState;

#endif /* STATE_H */
