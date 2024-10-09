#if _CMOC_VERSION_

#include <stdbool.h>
#include <coco.h>
#include <cmoc.h>

bool reverse=0;
char* cursor=0x400;
char ss[2] = {0,0};

unsigned char kbhit (void) { 
  return (unsigned char)inkey();
}

char cgetc (void) {
  return (char)waitkey(0);
}

void gotoxy (unsigned char x, unsigned char y) {
  cursor=(uint16_t)32*y+0x400+x;
}

void cputc (char c) {
  ss[0]=c;
  cputs(ss);
}

// Print string to screen
void cputs (char* s) {
  unsigned char c;

  while(c=*s++) {
    if (c=='\n') {
      cursor=cursor+ textScreenWidth - (((uint16_t)cursor-0x400) % textScreenWidth);
    } else if (c!='\r') {
      // Shift lowercase
      if (c>=0x60 && c<0x80)
        c-=0x20;
      
      // For text characters, handle reverse
      if (c<0x80) {
        // Reverse
        c|=0x40;

        // Normal
        if (!reverse)
          c^=0x40;
      }

      *cursor++=c;
    }
  }
}

/* Enable/disable reverse character display.
*/
unsigned char revers (unsigned char onoff) {
  return !(reverse = onoff);
}

/* Clear part of a line (write length spaces). */
void cclear (unsigned char length) {
  memset(cursor,reverse ? 0x60 : 0x20,length);
  cursor+=length;
}

/* Return the current screen size. */
void  screensize (unsigned char* x, unsigned char* y) {
  *x = 32; //textScreenWidth;
  *y = 16;// textScreenHeight;
}

/* Clear the whole screen and put the cursor into the top left corner */
void clrscr (void) {
  //uint8_t i;for(i=0;i<255;i++) *((uint8_t*)0x500+i)=i;
  memset(cursor=0x400,0x20,0x200);
}
#endif
