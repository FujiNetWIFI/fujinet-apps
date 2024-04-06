/**
 * @brief   ISS Tracker
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose UNIX timestamp transform functions.
 */

#ifndef TIMESTAMP_H
#define TIMESTAMP_H

/**
 * @brief write human readable date/time from unix timestam p
 * @param seconds The UNIX timestamp (64-bit)
 * @param tso The output area for timestamp (12 chars needed) 
 */
void timestamp(long seconds, char *tso);

#endif /* TIMESTAMP_H */
