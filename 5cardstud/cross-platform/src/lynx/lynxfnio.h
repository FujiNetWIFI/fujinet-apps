/**
 *  for Atari Lynx
 *
 * @brief I/O routines
 * @author Thom Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 */

#ifndef LYNXFNIO_H
#define LYNXFNIO_H

// Devices
#define DISK_DEV  0x04      // disk device
#define NET_DEV 	0x0E			// network device
#define FN_DEV		0x0F			// Fujinet device

// Max message size
#define LEN_MAX		1024

// Control codes
#define MN_RESET 	0x00   			// command.control (reset)
#define MN_STATUS 	0x10  			// command.control (status)
#define MN_ACK 		0x20     		// command.control (ack)
#define MN_CLR 		0x30     		// command.control (clr) (aka CTS)
#define MN_RECEIVE 	0x40 			// command.control (receive)
#define MN_CANCEL 	0x50  			// command.control (cancel)
#define MN_SEND 	0x60    		// command.control (send)
#define MN_NACK 	0x70    		// command.control (nack)
#define MN_READY 	0xD0   			// command.control (ready)

#define NM_STATUS 	0x80 			// response.control (status)
#define NM_ACK 		0x90    		// response.control (ack)
#define NM_CANCEL 	0xA0 			// response.control (cancel)
#define NM_SEND 	0xB0   			// response.data (send)
#define NM_NACK 	0xC0   			// response.control (nack)

#define NET_ACK		(NM_ACK & NET_DEV)
#define NET_NACK	(NM_NACK & NET_DEV)
#define DISK_ACK    (NM_ACK & DISK_DEV)
#define DISK_NACK   (NM_NACK & DISK_DEV)

// device type
#define DEVTYPE_CHAR     0
#define DEVTYPE_BLOCK    1

// Status codes
#define STATUS_OK        0
#define STATUS_BAD_BLOCK 1
#define STATUS_NO_BLOCK  2
#define STATUS_NO_MEDIA  3
#define STATUS_NO_DRIVE  4


// structs
typedef struct {
  unsigned char dev_stat;
  unsigned short max_msgsize;
  unsigned char type;
  unsigned char status;
  unsigned char ck;
} FUJI_IO_STATUS;


// Some globals to help with code size/speed (could be moved to zero page)
extern unsigned char _ck;			// checksum byte
extern char _r;						// response/data from FN


// helper functions
void _checksum(char *b, unsigned short len);
void _serial_get_loop(void);


unsigned char fnio_init(void);
unsigned char fnio_done(void);
void fnio_reset(unsigned char dev);
unsigned char fnio_status(unsigned char dev, unsigned char *buf);
unsigned char fnio_send(unsigned char dev, char *buf, unsigned short len);
unsigned char fnio_recv(unsigned char dev, char *buf, unsigned short *len);


#endif /* FUJINET_H */
