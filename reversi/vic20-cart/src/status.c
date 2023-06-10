/**
 * @brief #FujiNet Reversi
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <string.h>
#include <stdlib.h>
#include <cbm.h>

#define CMD 15

/**
 * @brief command to get status of channel 2
 */
const char *_status_cmd = "status,2";

/**
 * @brief Return status. Command channel must be open.
 * @param bw pointer to bytes waiting variable.
 * @param connected pointer to connected variable
 * @param error pointer to error pointer
 * @return false if error occured (you should read error variable)
 */
unsigned char status(unsigned short *bw, unsigned char *connected, unsigned char *error)
{
  char tmp[16];
  char *p;
  unsigned char e;

  memset(tmp,0,sizeof(tmp));

  cbm_write(CMD,_status_cmd,sizeof(_status_cmd));
  cbm_read(CMD,tmp,sizeof(tmp));

  p = strtok(tmp,",");
  if ((p != NULL) && (bw != NULL))
    *bw = atoi(p);

  p = strtok(NULL,",");
  if ((p != NULL) && (connected != NULL))
    *connected = atoi(p);
  
  p = strtok(NULL,",");
  e = atoi(p);
  if ((p != NULL) && (error != NULL))
    *error = e;

  return (e == 1);
}
