#ifndef APP_KEY_H
#define APP_KEY_H

#include <stdbool.h>

/* Bill Kendrick's App Key "Creator ID"
   (see https://github.com/FujiNetWIFI/fujinet-platformio/wiki/SIO-Command-%24DC-Open-App-Key) */

#define CREATOR_ID 0xB177

/* FujiNet ISS Tracker "App ID" & "Key ID" */
#define APP_ID 0x02
#define KEY_ID 0x01

bool read_settings(void);
bool write_settings(void);

#endif // APP_KEY_H
