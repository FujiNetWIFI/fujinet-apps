/**
 * From #FujiNet Network Testing tools
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Released under GPL 3.0
 * See COPYING for details.
 */

#ifndef NSIO_H
#define NSIO_H

/* From channelMode in fujinet-platformio/lib/device/sio/network.h */

#define CHANNELMODE_PROTOCOL 0
#define CHANNELMODE_JSON 1


/**
 * Open a file on N: device
 * @param unit the SIO unit number
 * @param buf the target buffer
 * @param aux1 the AUX1 value to use on open (read/write/etc.)
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

/**
 * Change the mode of previously-opened Nu:
 * @param unit the SIO unit number
 * @param aux1 the AUX1 value (same as what was used to open (read/write/etc.))
 * @param mode the mode to change to (CHANNELMODE_PROTOCOL or CHANNELMODE_JSON)
 * @return the error code
 */
unsigned char nchanmode(unsigned char unit, unsigned char aux1, unsigned char mode);

/**
 * Read and parse JSON from previously-opened Nu:
 * (Note: the N: device must be in JSON bmode; see nchanmode())
 * @param unit the SIO unit number
 * @param aux1 the AUX1 value (same as what was used to open (read/write/etc.))
 * @return the error code
 */
unsigned char njsonparse(unsigned char unit, unsigned char aux1);

/**
 * Query JSON from Nu:
 * (Note: the JSON must be parsed first; see njsonparse().
 * Then the data you've requested should be read; see nread().)
 * @param unit the SIO unit number
 * @param aux1 the AUX1 value (same as what was used to open (read/write/etc.))
 * @param buf buffer containing the path of JSON data to query
 * @return the error code
 */
unsigned char njsonquery(unsigned char unit, unsigned char aux1, char *buf);

#endif /* NSIO_H */
