/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose clock device functions
 */

#include <string.h>
#include <cbm.h>

#define CLFN 3
#define CDEV 29
#define CCMD 15

/**
 * @brief send formatted time string f to string s, derived from timestamp string t
 * @param s The destination string
 * @param f The strftime formatted string
 * @param t The timestamp value as string
 */
void dt(char *s, const char *f, const char *t)
{
  cbm_open(CLFN,CDEV,CCMD,t);
  cbm_write(CLFN,f,strlen(f));
  cbm_read(CLFN,s,256);
  cbm_close(CLFN);
}
