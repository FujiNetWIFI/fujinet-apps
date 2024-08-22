#ifndef NET_H
#define NET_H

typedef unsigned char byte;

/**
 * @brief get last error code
 * @param devid Device ID (0-255)
 * @return error code byte
 */
byte net_error(byte devid);

/**
 * @brief Open network connection
 * @param devid Device ID (0-255)
 * @param mode Open Mode (4=READ, 6=DIR, 8=WRITE, 9=APPEND, 12=READ/WRITE, 13=POST) 
 * @param trans Translation mode (0=none) 
 * @param url 256 byte max, N: URL
 * @return result code.
 */
byte net_open(byte devid, byte mode, byte trans, const char *url);

/**
 * @brief Close network connection
 * @param devid Device ID (0-255)
 */
void net_close(byte devid);

typedef struct _network_status
{
    unsigned short bytesWaiting;
    byte connected;
    byte error;
} NetworkStatus;

/**
 * @brief Return Network status
 * @param devid Device ID (0-255)
 * @param ns Pointer to NetworkStatus struct
 * @return Error result.
 */
byte net_status(byte devid, NetworkStatus *ns);

/**
 * @brief read from network channel
 * @param devid Device ID (0-255)
 * @param buf Destination buffer (must be able to hold len bytes)
 * @param len # of bytes to read (0-65535)
 * @return error result.
 */
byte net_read(byte devid, byte *buf, unsigned short len);

/**
 * @brief Set network channel mode
 * @param devid Device ID (0-255)
 * @param channelMode channel mode, 0 = normal, 1 = JSON
 */
void net_set_channel_mode(byte devid, byte channelMode);

/**
 * @brief Parse JSON waiting in channel
 * @param devid Device ID (0-255)
 * @return Result code (1=success, 213 = could not parse JSON)
 */
byte net_parse_json(byte devid);

/**
 * @brief query JSON
 * @param devid Device ID (0-255) 
 * @param qs pointer to query string 
 */
void net_set_json_query(byte devid, const char *qs);

#endif /* NET_H */
