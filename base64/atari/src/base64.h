/**
 * @brief base64 FujiNet Functions
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef BASE64_H
#define BASE64_H

#define BASE64_DEVICE 0x70
#define BASE64_TIMEOUT 0x02
#define BASE64_SUCCESS 1
#define BASE64_ERROR 144
#define BASE64_EXTENDED_ERROR_OFFSET 3

unsigned char base64_encode_input(char* buf, unsigned short len);
unsigned char base64_encode_compute(void);
unsigned char base64_encode_length(unsigned long *len);
unsigned char base64_encode_output(char* buf, unsigned short len);
unsigned char base64_decode_input(char* buf, unsigned short len);
unsigned char base64_decode_compute(void);
unsigned char base64_decode_length(unsigned long *len);
unsigned char base64_decode_output(char* buf, unsigned short len);

#endif /* BASE64_OAH */
