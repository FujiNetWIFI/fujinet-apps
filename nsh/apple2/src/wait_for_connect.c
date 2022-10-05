/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include "network.h"
#include "wait_for_connect.h"

#define RETRY_COUNT 128
#define CONNECTED 1 // Bit 2

bool _wait_for_connect(unsigned char bit)
{
  bool retries = RETRY_COUNT;
  
  // Wait until connection established
  while (retries > 0)
    {
      csleep(5);
      if (network_statusbyte() & bit)
	return true;
      else
	retries--;
    }

  if (retries == 0)
    {
      printf("COULD NOT OPEN NET CONNECTION.\n");
      return false;
    }
}

bool wait_for_connect(void)
{
  return _wait_for_connect(1);
}

bool wait_for_connect2(void)
{
  return _wait_for_connect(2);
}

