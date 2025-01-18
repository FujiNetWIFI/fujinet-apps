/**
 * @brief NGET - Copy from network to local
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef NGET_H
#define NGET_H

/**
 * @brief Do network transfer to local filesystem
 * @param source The source N: URL
 * @param dest the destination local file
 * @return error code
 */
int nget(char *source, char *dest);

#endif 
