/**
 * Network Testing tools
 *
 * ncopy - copy files to/from N:
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Released under GPL 3.0
 * See COPYING for details.
 */

#ifndef NSIO_H
#define NSIO_H

/**
 * Open a file on N: device
 * @param unit the SIO unit number
 * @param buf the target buffer
 * @param aux1 the AUX1 value to use on open (read/write)
 * @return error code
 */
unsigned char nopen(unsigned char unit, char* buf, unsigned char aux1);

/**
 * Close a file on the N: device
 * @param unit the SIO unit number
 * @return error code
 */
unsigned char nclose(unsigned char unit);

/**
 * Read requested (aux) number of bytes from N:
 * @param unit the SIO unit number
 * @param buf the destination buffer
 * @param len # of bytes to read
 * @return error code
 */
unsigned char nread(unsigned char unit, char* buf, unsigned short len);

/**
 * Write requested (aux) number of bytes to N:
 * @param unit the SIO unit number
 * @param buf the source buffer
 * @param len # of bytes to write
 * @return error code
 */
unsigned char nwrite(unsigned char unit, char* buf, unsigned short len);

/**
 * Get status of Nu: and return in OS.dvstat[]
 * @param unit the SIO unit number
 * @return the error code
 */
unsigned char nstatus(unsigned char unit);

#endif /* NSIO_H */
