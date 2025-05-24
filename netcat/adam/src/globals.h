/**
 * @brief FujiLink, a terminal emulator for FujiNet
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose global variables
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#define NET_DEV 0x09

/**
 * @brief program state
 */
typedef enum _state
  {
    SPLASH,
    DIRECTORY,
    TERM,
    HALT
  } State;

/**
 * @brief Terminal type
 */
typedef enum _terminalType
  {
    DUMB,
    VT100_64COL,
    VT100_80COL
  } TerminalType;

/**
 * @brief current program state
 */
extern State state;

/**
 * @brief a directory entry
 */
typedef struct _directory
{
  char name[12];
  char url[256];
  TerminalType type;
} Directory; // 63 bytes total.

/**
 * @brief The currently dialed entry
 */
extern Directory dialed_entry;

#endif /* GLOBALS_H */
