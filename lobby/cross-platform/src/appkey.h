/**
 * @brief   High Level App Key function that must be implemented by each platform
 * @author  Oscar Fowler, Thomas Cherryhomes, Eric Carr
 * @license gpl v. 3
 */

#ifndef APPKEY_H
#define APPKEY_H

/* ******************************************************************** 
 * Common defs and vars to be used by each implementation.
 * ********************************************************************/

#define MAX_APPKEY_LEN 64
#define APPKEY_READ  0
#define APPKEY_WRITE 1

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
        unsigned int length;
        unsigned char value[MAX_APPKEY_LEN];
        
    } read;
    struct
    {
        unsigned char value[MAX_APPKEY_LEN];
    } write;
} appkeyblock;


/* ********************************************************************
 * Functions that must be implemented by each platform
 * ********************************************************************/

/**
 * @brief Read an appkey value into the specified buffer
 * @param creator_id Key creator ID: ($0000-$FFFF)
 * @param app_id The App ID ($00-$FF)
 * @param key_id The Key ID ($00-$FF)
 * @param data Buffer to hold string result
 */
unsigned char read_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, unsigned char *data);

/**
 * @brief Write an appkey value from the specified buffer
 * @param creator_id Key creator ID: ($0000-$FFFF)
 * @param app_id The App ID ($00-$FF)
 * @param key_id The Key ID ($00-$FF)
 * @param data string to write into key
 */
unsigned char write_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, unsigned char *data);

#endif /* APPKEY_H */
