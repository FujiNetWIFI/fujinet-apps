/**
 * @brief #FujiNet Reversi
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef STATUS_H
#define STATUS_H

/**
 * @brief Return status. Command channel must be open.
 * @param bw pointer to bytes waiting variable.
 * @param connected pointer to connected variable
 * @param error pointer to error pointer
 * @return false if error occured (you should read error variable)
 */
unsigned char status(unsigned short *bw, unsigned char *connected, unsigned char *error);

#endif /* STATUS_H */
