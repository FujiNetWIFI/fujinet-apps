/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * io.h - Input/output functions (serial/ethernet)
 */

#ifndef IO_H
#define IO_H

#define XON  0x11
#define XOFF 0x13

#include <stdbool.h>

/**
 * io_init() - Set-up the I/O
 */
void io_init(void);

/**
 * io_init_funcptrs() - Set up I/O function pointers
 */
void io_init_funcptrs(void);

/**
 * io_open() - Open the device
 */
void io_open(void);

/**
 * io_send_byte(b) - Send specified byte out
 */
void io_send_byte(unsigned char b);

/**
 * io_main() - The IO main loop
 * @return false = we've disconnected.
 */
bool io_main(void);

/**
 * io_recv_serial() - Receive and interpret serial data.
 */
void io_recv_serial(void);


/**
 * io_done() - Called to close I/O
 */
void io_done(void);

#endif /* IO_H */
