/**
 * @brief MS-DOS AppKey funcs
 * @author Thom Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl, v. 3
 */

#include <stdlib.h>
#include <stdbool.h>
#include <i86.h>
#include "../platform-specific/appkey.h"

#define FUJICMD_OPEN_APPKEY 0xDC
#define FUJICMD_WRITE_APPKEY 0xDE
#define FUJICMD_READ_APPKEY 0xDD

typedef struct _appkey
{
    unsigned short creator_id;
    unsigned char app_id;
    unsigned char key_id;
    unsigned char open_mode;
    unsigned char reserved;
} Appkey;

Appkey appkey;

/**
 * @brief Open a key for reading or writing
 * @param open_mode 0 = read, 1 = write
 * @param creator_id Key creator ID: ($0000-$FFFF)
 * @param app_id The App ID ($00-$FF)
 * @param key_id The Key ID ($00-$FF)
 * @return error code
 */
unsigned char open_appkey(unsigned char open_mode, unsigned int creator_id, unsigned char app_id, char key_id)
{
    union REGS r;
    struct SREGS sr;
    
    appkey.creator_id = creator_id;
    appkey.app_id = app_id;
    appkey.key_id = key_id;
    appkey.open_mode = open_mode;
    appkey.reserved = 0;
    
    r.h.dl = 0x80;
    r.h.al = 0x70;
    r.h.ah = 0xDC;
    r.h.cl = 0x00;
    r.x.cx = 0x00;
    r.x.si = 0x0000;
    sr.es = FP_SEG(&appkey);
    r.x.bx = FP_OFF(&appkey);
    r.x.di = sizeof(appkey);
    int86x(0xF5,&r,&r,&sr);

    delay(250);

    return r.h.al;
}

void read_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char* data)
{
    union REGS r;
    struct SREGS sr;

    open_appkey(0,creator_id,app_id,key_id);

    delay(250);

    r.h.dl = 0x40;
    r.h.al = 0x70;
    r.h.ah = 0xDD;
    r.x.cx = 64;
    r.x.si = 0x0000;
    sr.es = FP_SEG(data);
    r.x.bx = FP_OFF(data);
    r.x.di = 64;
    int86x(0xF5,&r,&r,&sr);
}

void write_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char *data)
{
    union REGS r;
    struct SREGS sr;
    
    open_appkey(1,creator_id,app_id,key_id);

    delay(250);

    r.h.dl = 0x80;
    r.h.al = 0x70;
    r.h.ah = 0xDE;
    r.x.cx = 64;
    r.x.si = 0x0000;
    sr.es = FP_SEG(data);
    r.x.bx = FP_OFF(data);
    r.x.di = 64;
    int86x(0xF5,&r,&r,&sr);
}

