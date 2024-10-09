#ifndef CONIO_H
#define CONIO_H

unsigned char kbhit (void);
char cgetc (void);
unsigned char kbhit (void);
void gotoxy (unsigned char x, unsigned char y);
void  cputc (char c);
void  cputs (const char* s);

/* Enable/disable reverse character display. This may not be supported by
** the output device. Return the old setting.
*/
unsigned char revers (unsigned char onoff);

/* Clear part of a line (write length spaces). */
void  cclear (unsigned char length);

/* Return the current screen size. */
void  screensize (unsigned char* x, unsigned char* y);

/* Clear the whole screen and put the cursor into the top left corner */
void clrscr (void);

#endif 
