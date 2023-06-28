/**
 * @brief   FujiNet News Reader - Commodore 16 Version
 * @author  Thomas Cherryhomes
 * @license gpl v. 3
 * @brief   network I/O functions
 */

#include <cbm.h>
#include <string.h>
#include <stdio.h>
#include "defines.h"

extern char buf[BUF_SIZE];

const char *urlBase="http://fujinet.online/8bitnews/news.php?t=lf&ps=39x24&l=5";

/**
 * @brief setup PETSCII translation for SAN
 */
void io_init(void)
{
  cbm_open(CMD,DEV,CMD,"settrans,2,4");
  cbm_close(CMD);
}

/**
 * @brief wrapper for network i/o
 * @verbose concatenate s onto baseURL, read result into buf
 * @param s URL fragment to concatenate onto urlBase
 */
void io_network(char *s)
{
  memset(buf,0,BUF_SIZE);

  sprintf(buf,"%s%s",urlBase,s);
  
  cbm_open(LFN,DEV,SAN,buf);

  memset(buf,0,BUF_SIZE);

  cbm_read(LFN,buf,BUF_SIZE);
  
  cbm_close(LFN);
}
