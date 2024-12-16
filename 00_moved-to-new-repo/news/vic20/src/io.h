/**
 * @brief   FujiNet News Reader - VIC-20 Cart Version
 * @author  Thomas Cherryhomes
 * @license gpl v. 3
 * @brief   network I/O functions
 */

#ifndef IO_H
#define IO_H

/**
 * @brief setup PETSCII translation for SAN
 */
void io_init(void);

/**
 * @brief wrapper for network i/o
 * @verbose concatenate s onto baseURL, read result into buf
 * @param s URL fragment to concatenate onto urlBase
 */
void io_network(char *s);

#endif /* IO_H */
