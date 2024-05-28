/**
 * @brief   Cbm App Key Functions
 * @author  Oscar Fowler, Thomas Cherryhomes, Eric Carr
 * @license gpl v. 3
 */

#ifndef APPKEY_H
#define APPKEY_H

#define MAX_APPKEY_LEN 64

#define APPKEY_READ  0
#define APPKEY_WRITE 1

// Uncomment this line to simulate read/writing app keys in VICE emulator on drive 11 (Must configure as EIC Host File System)
//#define USE_VICE_EMULATOR

/**
 * @brief Convenience type for handling an appkey
 */
typedef union {
    struct
    {
        unsigned int creator;
        unsigned char app;
        unsigned char key;
        unsigned char mode;
        unsigned char reserved;
    } open;
    struct
    {
        unsigned char length;
        unsigned char reserved;
        unsigned char value[MAX_APPKEY_LEN];
        
    } read;
    struct
    {
       char value[MAX_APPKEY_LEN+32];
    } write;
} appkeyblock;

/**
 * @brief Read a key into appkey var appkey
 * @param creator_id Key creator ID: ($0000-$FFFF)
 * @param app_id The App ID ($00-$FF)
 * @param key_id The Key ID ($00-$FF)
 * @brief string output (64 chars max)
 */
void read_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char *data);

/**
 * @brief write a key from appkey var appkey
 * @param creator_id Key creator ID: ($0000-$FFFF)
 * @param app_id The App ID ($00-$FF)
 * @param key_id The Key ID ($00-$FF)
 * @param Data string to write (64 chars max)
 */
void write_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char *data);

#endif /* APPKEY_H */
