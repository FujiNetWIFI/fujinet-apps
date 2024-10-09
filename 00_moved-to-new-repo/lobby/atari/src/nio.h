/**
 * N: I/O
 */

#ifndef NIO_H
#define NIO_H

#define DFUJI   0x71
#define DREAD   0x40
#define DWRITE  0x80
#define DUPDATE 0xC0

#define OREAD   0x04
#define OWRITE  0x08
#define OUPDATE 0x0C
#define OREADWRITE OUPDATE

#define SUCCESS 1
#define DERROR  144

#define DVSTAT_BYTES_WATING_LO  0
#define DVSTAT_BYTES_WATING_HI  1
#define DVSTAT_PROTOCOL		    2 
#define DVSTAT_EXTENDED_ERROR	3

/**
 * Open N: device with devicespec
 * @param devicespec - an N: device spec, e.g. N:TCP://FOO.COM:1234/
 * @param translation mode, 0=none, 1=cr, 2=lf, 3=cr/lf
 * @return error code, or 1 if successful.
 */
unsigned char nopen(char* devicespec, unsigned char trans);

/**
 * Close N: device with devicespec
 * @param devicespec - an N: device spec to close (the unit number is extracted)
 * @return error code, or 1 if successful.
 */
unsigned char nclose(char* devicespec);

/**
 * Get status of specific N: device
 * @param devicespec - an N: device spec to status (the unit number is extracted)
 * @return error code, or 1 if successful, DVSTAT is also filled with status info.
 *
 * Format of DVSTAT:
 * OS.dcb.dvstat[0] = # of bytes waiting LO
 * OS.dcb.dvstat[1] = # of bytes waiting HI
 * OS.dcb.dvstat[2] = reserved
 * OS.dcb.dvstat[3] = Error code of last I/O operation. !1 = error.
 */
unsigned char nstatus(char* devicespec);

/**
 * Read # of bytes from specific N: device.
 * @param devicespec - an N: device spec to read bytes from.
 * @param buf - The buffer to read into, must be at least as big as len.
 * @param len - The # of bytes to read (up to 65535)
 * @return error code, or 1 if successful, buf is filled with data.
 */
unsigned char nread(char* devicespec, unsigned char* buf, unsigned short len);

/**
 * Write # of bytes to specific N: device.
 * @param devicespec - an N: device spec to write to.
 * @param buf - The buffer to write to device, should be at least as big as len.
 * @param len - The # of bytes to write (up to 65535)
 * @return error code, or 1 if successful, buf is filled with data.
 */
unsigned char nwrite(char* devicespec, unsigned char* buf, unsigned short len);

/**
 * Send username and password credentials
 * @param devicespec - The devicespec.
 * @param login - The username to send
 * @param password - The password to send
 */
unsigned char nlogin(char* devicespec, char* login, char* password);


/**
 * Change the mode of previously-opened Nu:
 * @param unit the SIO unit number
 * @param aux1 the AUX1 value (same as what was used to open (read/write/etc.))
 * @param mode the mode to change to (CHANNELMODE_PROTOCOL or CHANNELMODE_JSON)
 * @return the error code
 */
unsigned char nchanmode(char* devicespec, unsigned char mode);

/**
 * Opens a connection, retrieves and parses Json, ready to be queried. Call nclose after querying
 * @param devicespec - an N: device spec, e.g. N:TCP://FOO.COM:1234/
 * @param translation mode, 0=none, 1=cr, 2=lf, 3=cr/lf
 * @return error code, or 1 if successful.
 */
unsigned char njsonparse(char* devicespec, unsigned char trans);

/**
 * Query JSON from N:
 * (Note: the JSON must be parsed first; see njsonparse().
 * Then the data you've requested should be read; see nread().)
 * @param devicespec - an N: device spec, e.g. N:TCP://FOO.COM:1234/
 * @param buf buffer containing the path of JSON data to query
 * @return the error code
 */
unsigned char njsonquery(char* devicespec, char *buf);

#endif /* NIO_H */
