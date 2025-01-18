/**
 * @brief NPUT - Copy from network to local
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef NPUT_H
#define NPUT_H

/**
 * @brief Do network transfer to network
 * @param source The source filename
 * @param dest the destination URL
 * @return error code
 */
int nput(char *source, char *dest);

#endif 
