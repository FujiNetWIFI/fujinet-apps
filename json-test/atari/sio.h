#ifndef SIO_H
#define SIO_H

#define SIO_READ 0x40
#define SIO_WRITE 0x80

/**
 * Function to call sio
 * @return the SIO error code
 */
unsigned char siov();

#endif // SIO_H

