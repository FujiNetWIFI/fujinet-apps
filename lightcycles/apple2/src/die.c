/**
 * @brief   Print message and immediately exit.
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "die.h"

/**
 * @brief Print message, and exit program
 * @param s \0 String to print.
 */
void die(const char *s)
{
  printf("%s\nPRESS ANY KEY TO CONTINUE.",s);
  cgetc();
  exit(0);
}
