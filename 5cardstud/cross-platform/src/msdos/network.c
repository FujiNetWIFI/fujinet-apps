#ifdef __WATCOMC__

#include <dos.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../misc.h"

unsigned char buf[256];

struct _status
{
    uint16_t bw;    // Bytes waiting
    uint8_t c;      // Connected?
    uint8_t err;    // last error.
} s;

uint16_t getJsonResponse(char *url, unsigned char *buffer, uint16_t max_len)
{
    union REGS r;
    struct SREGS sr;
    
    // Open
    strcpy(buf,url);
    r.h.dl = 0x80; // write
    r.h.al = 0x71; // Network device 1
    r.h.ah = 'O';  // open
    r.h.cl = 0x0c; // GET
    r.h.ch = 0x00; // No translation
    r.x.si = 0x0000; // aux3/4 not used
    sr.es = FP_SEG(buf);
    r.x.bx = FP_OFF(buf);
    r.x.di = sizeof(buf);
    int86x(0xF5,&r,&r,&sr);

    // Set channel mode to JSON
    r.h.dl = 0x00; // none
    r.h.al = 0x71; // Network device 1
    r.h.ah = 0xFC; // Set channel mode
    r.h.cl = 0x0c; // GET
    r.h.ch = 0x01; // JSON
    r.x.si = 0x0000; // not used
    int86(0xF5,&r,&r);
    
    // Parse
    r.h.dl = 0x00; // none
    r.h.al = 0x71; // Network device 1
    r.h.ah = 'P';  // Parse
    r.h.cl = 0x0c; // GET
    r.h.ch = 0x00; // No translation
    r.x.si = 0x0000; // Not used
    int86(0xF5,&r,&r);
    
    // query
    r.h.dl = 0x80; // Write
    r.h.al = 0x71; // Network Device 1
    r.h.ah = 'Q';  // Query
    r.h.cl = 0x0c; // Get
    r.h.ch = 0x00; // No trans
    r.x.si = 0x0000; // not used
    sr.es = FP_SEG(buf);
    r.x.bx = FP_OFF(buf);
    r.x.di = sizeof(buf);
    int86x(0xF5,&r,&r,&sr);

    // Status (# of bytes read)
    r.h.dl = 0x40; // Read
    r.h.al = 0x71; // Network device 1;
    r.h.ah = 'S';  // Status
    r.h.cl = 0x0c; // Get
    r.h.ch = 0x00; // No translation
    r.x.si = 0x0000; // Not used
    sr.es = FP_SEG(&s);
    r.x.bx = FP_OFF(&s);
    r.x.di = sizeof(s);
    int86x(0xF5,&r,&r,&sr);
    
    // Read
    r.h.dl = 0x40; // Read
    r.h.al = 0x71; // Network Device 1
    r.h.ah = 'R';  // Read
    r.x.cx = s.bw; // # of bytes to read
    r.x.si = 0x0000; // Not used
    sr.es = FP_SEG(buffer);
    r.x.bx = FP_OFF(buffer);
    r.x.di = s.bw; // # of bytes to read
    int86x(0xF5,&r,&r,&sr);
    
    // close
    r.h.dl = 0x00; // None
    r.h.al = 0x71; // Network Device 1
    r.h.ah = 'C';  // Close
    r.x.cx = 0x0000; // Not used
    r.x.si = 0x0000; // Not used
    int86(0xF5,&r,&r);
    
    return 0;
}

#endif
