/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Utility functions
 */

#include <string.h>

#define UTIL_CENTER_TERMINAL_WIDTH 80

/**
 * @brief center text relative to 80 columns
 * @param s The string to return X value
 * @return x value
 */
unsigned char util_center(const char *s)
{
  unsigned char l = strlen(s);
  
  return (l >= UTIL_CENTER_TERMINAL_WIDTH) ? 0 : (UTIL_CENTER_TERMINAL_WIDTH - l) >> 1;
}
