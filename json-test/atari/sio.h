#ifndef SIO_H
#define SIO_H

#define SIO_READ 0x04
#define SIO_WRITE 0x08
#define SIO_DIRECTORY 0x02
#define SIO_APPEND 0x01

/**
 * Function to call sio
 * @return the SIO error code
 */
unsigned char siov();

#endif // SIO_H

