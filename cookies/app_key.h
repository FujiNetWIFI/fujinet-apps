#ifndef APP_KEY_H
#define APP_KEY_H

#define MAX_APPKEY_LEN 64

/* An App Key "Creator ID" needs to be specified!
   (see https://github.com/FujiNetWIFI/fujinet-platformio/wiki/SIO-Command-%24DC-Open-App-Key) */

#define CREATOR_ID 0xFFFF

#define APP_ID 0x00
#define KEY_ID 0x00

void read_settings(void);
unsigned char write_settings(void);

#endif // APP_KEY_H
