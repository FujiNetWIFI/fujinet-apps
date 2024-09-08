
/**
 * @brief FujiNet Network Device Library
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_NETWORK_H
#define FUJINET_NETWORK_H

#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#else
#include <stdint.h>
#include <stdbool.h>
#endif /* _CMOC_VERSION_ */

#ifdef __CBM__

// For DATA transfers, we want to undo some of the charmap settings in CC65.
// So map the characters in the range 5b-60, 7b-7f back to themselves, as we want to send them as-is to FujiNet.
// This undoes some "look alike" character that move into 0x80+ range, which in FN are converted to 3 byte utf-8 chars incorrectly.

#pragma charmap (0x5B, 0x5B)
#pragma charmap (0x5C, 0x5C)
#pragma charmap (0x5D, 0x5D)
#pragma charmap (0x5E, 0x5E)
#pragma charmap (0x5F, 0x5F)
#pragma charmap (0x60, 0x60)
#pragma charmap (0x7B, 0x7B)
#pragma charmap (0x7C, 0x7C)
#pragma charmap (0x7D, 0x7D)
#pragma charmap (0x7E, 0x7E)
#pragma charmap (0x7F, 0x7F)

#endif


/**
 * The number of bytes read in the last call to network_read().
 * This can be less than the amount asked for, if there aren't enough bytes available from target.
 * This allows applications to add nul terminators etc.
 */
extern uint16_t fn_bytes_read;

/**
 * Device specific error. This is the raw code from any device errors before they are converted to
 * simpler device-agnostic network library errors.
 */
extern uint8_t fn_device_error;

/**
 * Convert device specific error in code to FujiNet Network library error, agnostic of device.
 * Library code calls this when it encounters an error to return value applications should use.
 */
uint8_t fn_error(uint8_t code);

/*
 * Network status values. These are set during network_read. You can capture your own using network_status.
 * bw      : bytes waiting
 * conn    : Connected status, this is 1 for still reading a particular resource, 0 when the current read has completed.
 * error   : the error status returned from FN, e.g. 136 for EOF, 1 for normal OK status (don't ask why)
 */
extern uint16_t fn_network_bw;
extern uint8_t fn_network_conn;
extern uint8_t fn_network_error;

/**
 * @brief  Initialise network device
 * Allows initialisation of network to perform any platform dependent checks, and allow applications to
 * exit early if there is a network issue.
 * @return fujinet-network status/error code (See FN_ERR_* values) and set device specific error if there is any
 */
uint8_t network_init();

/**
 * @brief  Get Network Device Status byte
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param  bw pointer to where to put bytes waiting
 * @param  c pointer to where to put connection status
 * @param  err to where to put network error byte.
 * @return fujinet-network status/error code (See FN_ERR_* values)
 */
uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);

/**
 * @brief  Close Connection
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_close(char* devicespec);

/**
 * @brief  Open Connection
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param  mode (4=read, 8=write, 12=read/write, 13=POST, etc.)
 * @param  trans translation mode (CR/LF to other line endings; 0=none, 1=CR, 2=LF, 3=CRLF, 4=Pet)
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_open(char* devicespec, uint8_t mode, uint8_t trans);

/**
 * @brief  Non-blocking read from channel
 * 
 * The read will grab whatever is waiting in the FujiNet buffer. If fewer than the requested len, the return count will reflect this.
 * Errors are returned as the negative value of the FUJI standard error. fn_network_error contains the device specific error code. fn_bytes_read will be 0 on errors.
 * 
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  buf Buffer
 * @param  len length
 * @return Bytes read, or negative value of fujinet-network error code (See FN_ERR_* values) with fn_network_error containing real error code
 */
int16_t network_read_nb(char* devicespec, uint8_t *buf, uint16_t len);

/**
 * @brief  Read from channel
 * 
 * The read will block until it has read all the bytes requested from the device, or the EOF is hit.
 * This will block waiting for as much data as it can, so that the client does not need to handle counting.
 * Errors are returned as the negative value of the error. fn_network_error contains the device specific error code. fn_bytes_read will contain the count of bytes read before error occurred.
 * 
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  buf Buffer
 * @param  len length
 * @return Bytes read, or negative value of fujinet-network error code (See FN_ERR_* values) with fn_network_error containing real error code
 */
int16_t network_read(char* devicespec, uint8_t *buf, uint16_t len);

/**
 * @brief  Write to network 
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  buf Buffer
 * @param  len length
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_write(char* devicespec, uint8_t *buf, uint16_t len);

/**
 * @brief  Device specific direct control commands
 * @param  cmd Command byte to send
 * @param  aux1 Auxiliary byte 1
 * @param  aux2 Auxiliary byte 2
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  ... varargs - Device specific additional parameters to pass to the network device
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, ...);

/**
 * @brief  Parse the currently open JSON location
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * This will set the channel mode to JSON, which will be unset in the close.
 */
uint8_t network_json_parse(char *devicespec);

/**
 * @brief  Perform JSON query
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  query pointer to string containing json path to query, e.g. "/path/field". No need to add device drive.
 * @param  s pointer to receiving string, nul terminated, if no data was retrieved, sets it to an empty string
 * @return Bytes read, or negative values represent fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open and parsed json.
 */
int16_t network_json_query(char *devicespec, char *query, char *s);

/**
 * @brief  Sets the channel mode.
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  mode The mode to set
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
uint8_t network_http_set_channel_mode(char *devicespec, uint8_t mode);

/**
 * @brief  Start adding headers.
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection. After calling this, add any headers with network_http_add_header, and finally call network_http_end_add_headers
 */
uint8_t network_http_start_add_headers(char *devicespec);

/**
 * @brief  End adding headers.
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection. Completes header adding, and sets mode back to BODY
 */
uint8_t network_http_end_add_headers(char *devicespec);

/**
 * @brief  Add header to HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  header pointer to string containing full header to add, e.g. "Accept: application/json"
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
uint8_t network_http_add_header(char *devicespec, char *header);


/**
 * @brief  Send POST HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  data data to post
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
uint8_t network_http_post(char *devicespec, char *data);

/**
 * @brief  Send PUT HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  data data to put
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
uint8_t network_http_put(char *devicespec, char *data);

/**
 * @brief  Send DELETE HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  trans translation value
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * This will open a connection, consumer can then query the data, and must close the connection.
 */
uint8_t network_http_delete(char *devicespec, uint8_t trans);

/**
 * @brief  Internal routine to get the network UNIT id from the devicespec, i.e. Nx: find the "x" value
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return unit number
 * 
 */
uint8_t network_unit(char *devicespec);


#define FN_ERR_OK               (0x00)      /* No error */
#define FN_ERR_IO_ERROR         (0x01)      /* There was IO error/issue with the device */
#define FN_ERR_BAD_CMD          (0x02)      /* Function called with bad arguments */
#define FN_ERR_OFFLINE          (0x03)      /* The device is offline */
#define FN_ERR_WARNING          (0x04)      /* Device specific non-fatal warning issued */
#define FN_ERR_NO_DEVICE        (0x05)      /* There is no network device */

#define FN_ERR_UNKNOWN          (0xff)      /* Device specific error we didn't handle */

#define OPEN_MODE_READ          (0x04)
#define OPEN_MODE_WRITE         (0x08)
#define OPEN_MODE_RW            (0x0C)

#define OPEN_MODE_HTTP_GET      (0x04)
#define OPEN_MODE_HTTP_PUT      (0x08)
#define OPEN_MODE_HTTP_GET_H    (0x0C)
#define OPEN_MODE_HTTP_POST     (0x0D)
#define OPEN_MODE_HTTP_PUT_H    (0x0E)
#define OPEN_MODE_HTTP_DELETE   (0x05)
#define OPEN_MODE_HTTP_DELETE_H (0x09)

#define CHANNEL_MODE_JSON       (0xFC)

#define OPEN_TRANS_NONE         (0x00)
#define OPEN_TRANS_CR           (0x01)
#define OPEN_TRANS_LF           (0x02)
#define OPEN_TRANS_CRLF         (0x03)
#define OPEN_TRANS_PET          (0x04)

#define HTTP_CHAN_MODE_BODY             (0)
#define HTTP_CHAN_MODE_COLLECT_HEADERS  (1)
#define HTTP_CHAN_MODE_GET_HEADERS      (2)
#define HTTP_CHAN_MODE_SET_HEADERS      (3)
#define HTTP_CHAN_MODE_POST_SET_DATA    (4)

#endif /* FUJINET_NETWORK_H */
