#ifndef APP_KEY_H
#define APP_KEY_H

#define MAX_APPKEY_LEN 64

/* Bill Kendrick's App Key "Creator ID"
   (see https://github.com/FujiNetWIFI/fujinet-platformio/wiki/SIO-Command-%24DC-Open-App-Key) */

#define CREATOR_ID 0xB177

/* FujiNet APOD viewer "App ID" & "Key ID" */
#define APP_ID 0x01
#define KEY_ID 0x01

void read_settings(void);
unsigned char write_settings(void);

#endif // APP_KEY_H
