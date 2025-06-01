#include <i86.h>
#include <dos.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../misc.h"

void waitvsync(void);

// Comment out this for real fujinet builds. This is just for eric's simple "get basic https working" emulator bridge
// #define EMU_MODE 1

unsigned char buf[256];

struct _status
{
    uint16_t bw;    // Bytes waiting
    uint8_t c;      // Connected?
    uint8_t err;    // last error.
} s;

/* Because I don't want to drag all of conio into this project! */
_WCIRTLINK extern unsigned inp(unsigned __port);
_WCIRTLINK extern unsigned outp(unsigned __port, unsigned __value);

uint16_t getResponse(char *url, unsigned char *buffer, uint16_t max_len)
{
    union REGS r;
    struct SREGS sr;
    int i=0, j=0;

    memset(buf,0,sizeof(buf));

    for (i=0;i<strlen(url);i++)
    {
        if (url[i]==0x0d || url[i]==0x00)
            continue;
        buf[j++]=url[i];
    }

    memset(&r,0,sizeof(union REGS));
    memset(&sr,0,sizeof(struct SREGS));
    
    // Open
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

    delay(250);
    
    memset(&r,0,sizeof(union REGS));
    memset(&sr,0,sizeof(struct SREGS));

    // Status (# of bytes read)
    r.h.dl = 0x40; // Read
    r.h.al = 0x71; // Network device 1;
    r.h.ah = 'S';  // Status
    r.h.cl = 0x00; // Get
    r.h.ch = 0x00; // No translation
    r.x.si = 0x0000; // Not used
    sr.es = FP_SEG(&s);
    r.x.bx = FP_OFF(&s);
    r.x.di = sizeof(s);
    int86x(0xF5,&r,&r,&sr);

    delay(250);
    
    memset(&r,0,sizeof(union REGS));
    memset(&sr,0,sizeof(struct SREGS));

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

    delay(250);
    
    memset(&r,0,sizeof(union REGS));
    memset(&sr,0,sizeof(struct SREGS));

    // close
    /* r.h.dl = 0x00; // None */
    /* r.h.al = 0x71; // Network Device 1 */
    /* r.h.ah = 'C';  // Close */
    /* r.x.cx = 0x0000; // Not used */
    /* r.x.si = 0x0000; // Not used */
    /* int86x(0xF5,&r,&r,&sr); */

    /* delay(250); */
    
    memset(&r,0,sizeof(union REGS));
    memset(&sr,0,sizeof(struct SREGS));

    return s.bw;
}



#ifdef EMU_MODE

// Simple "get https working in dosbox" code to retrieve a url until fujinet-pc for msdos is available.
// I have the following line set in dosbox to map COM1 to a service listening on TCP port 5000 that makes the https call
// serial1=nullmodem server:127.0.0.1 port:5000

#define COM1    0x3F8
#define LSR     5
#define LSR_THR_EMPTY 0x20
#define LSR_DATA_READY 0x01

void send_char(uint8_t c) {
    while (!(inp(COM1 + LSR) & LSR_THR_EMPTY));
    outp(COM1, c);
}

uint8_t recv_char() {
    uint8_t b;
    while (!(inp(COM1 + LSR) & LSR_DATA_READY));
    b= inp(COM1);

    // Decode byte if needed - 0xFF doesn't get sent, so encode it using 0xFE.
    // if 0xFE is read, read one more byte and add to it
    // This results in: 0xFE00 = 0xFE and 0xFE01 = 0xFF
    if (b==0xfe) {
      while (!(inp(COM1 + LSR) & LSR_DATA_READY));
      b+=inp(COM1);
    }
    return b;
}


/// @brief Retrieve the response
/// @param url
/// @return response length
uint8_t getResponse(char *url, unsigned char *buffer, uint16_t max_len) {
  uint16_t count, i;

  send_char('O');
  while(*url)
    send_char(*url++);
  send_char(0);

  // read until we get to 0xfd start byte
  while ( (count = recv_char()) != 0xfd);

  count = recv_char();
  count*=256;
  count += recv_char();

  for(i=0;i<count;i++) {
    *buffer=recv_char();
    buffer++;
  }

  return count>0;
}

#endif
