/**
 * @brief FujiNet Lobby for Adam
 * @author Thomas Cherryhomes, Geoff Oltmans
 * @email thom dot cherryhomes at gmail dot com or oltmansg at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include "chat.h"
#include "state.h"
#include <smartkeys.h>
#include "input.h" 
#include <stdlib.h>
#include <eos.h>
#include <video/tms99x8.h>
#include <conio.h>
#include "cursor.h"

#define CHAT_Y  19

extern State state;
unsigned char chat_rx_buf[128];
char chat_tx_buf[64];
extern char _username[32];
unsigned int pos = 0;
unsigned char x,y;
unsigned char len;
unsigned char input_x;
unsigned char input_y;
bool myKeyVI = false;
int i=0;
unsigned char scrollbuf[0x0100];
const char Lobby[] = ">#Lobby>";

#define CHAT_DEV 0x0A

static void scrollscreen(unsigned char numLines)
{
  unsigned int vdpaddr = numLines*0x0100;

  if (numLines == 0 || numLines > CHAT_Y)
    return;

  for (;vdpaddr<=CHAT_Y*0x0100;vdpaddr+=0x0100)
  {
    vdp_vread(vdpaddr, scrollbuf, 0x0100);
    vdp_vwrite(scrollbuf, vdpaddr-0x0100, 0x0100);
  }

  vdp_vfill((CHAT_Y - numLines) * 0x0100,0x00,numLines*0x0100); // Clear scrolled lines at bottom.

}

static void rxFromNet(void)
{
  char *p = NULL;
  unsigned char i,newLines=1;
  // char *myName = NULL;

  while (eos_read_character_device(CHAT_DEV,chat_rx_buf,strlen(chat_rx_buf)) < 0x80);
  p = strtok((char *)chat_rx_buf,"\n");

  while (p)
  {
      // myName = strstr(p,(char *)username);
      // if ((myName != NULL) && (myName != p+1)) // Check to see if someone mentioned me
      //   smartkeys_sound_play(SOUND_POSITIVE_CHIME);
      
      i = strlen(p);
      if (strstr(p,Lobby) == p ) // skip the >#Lobby> if it's there...
        p+=strlen(Lobby);
      // if (i+2+strlen(_username)>64)  // our input buffer only holds two lines
      //   newLines = 2;

      if (y + (newLines*8) >= CHAT_Y*8)
      {
          x=0;
          y=(CHAT_Y-newLines)*8;
          scrollscreen(newLines);
      } 

      smartkeys_puts(x,y,p);
      y += newLines*8;

      p = strtok(NULL,"\n");
      if (p) {
        csleep(1);
      }
  }
}

void processKeyboard(void)
{
  unsigned char key;
  int length;
 
  key = input();
  gotoxy(input_x,input_y);

  if (key == KEY_RETURN)
  {
    chat_tx_buf[i++] = '\n';
    chat_tx_buf[i++] = 0;
    length = strlen(chat_tx_buf);

    eos_write_character_device(CHAT_DEV,chat_tx_buf,length);
    i=8;
    pos=0;
    input_x=0;
    input_y=CHAT_Y;
    smartkeys_sound_play(SOUND_POSITIVE_CHIME);
    cursor_pos(input_x,input_y);
    gotoxy(input_x,input_y);
    vdp_vfill(0x1200,0x00,768); //clear the input line
  }
  else if (key == KEY_BACKSPACE)
  {
    if (pos > 0)
      {
        pos--;
        input_x--;
        i--;
        chat_tx_buf[i] = 0x00;
        smartkeys_sound_play(SOUND_TYPEWRITER_CLACK);

        putchar(KEY_BACKSPACE);
        putchar(KEY_SPACE);
        putchar(KEY_BACKSPACE);
        cursor_pos(input_x,input_y);
      }
  }
  else if (key == KEY_SMART_VI)
  {
    smartkeys_sound_play(SOUND_MODE_CHANGE);
    state = FETCH;
    return;
  }
  else if (key > 0x1F && key < 0x7F) // Printable characters 
  {
    if (pos < len)
      {
        pos++;
        input_x++;
        chat_tx_buf[i++] = key;
        // i++;
        smartkeys_sound_play(SOUND_KEY_PRESS);

        putchar(key);
        cursor_pos(input_x,input_y);
      }
  }
  input_x = wherex();
  input_y = wherey();
}


void chat(void)
{

  x = 0;
  y = 0;
  input_x = 0;
  input_y = CHAT_Y;
  clrscr();

  smartkeys_clear();
  smartkeys_display(NULL,NULL,NULL,NULL,NULL," EXIT");
  smartkeys_status("\n  CHAT");

  eos_start_read_keyboard(); //just to kick scanning off on keyboard...
  memset(chat_tx_buf,0,sizeof(chat_tx_buf));
  i = sprintf(chat_tx_buf, "W#Lobby ");
  len = sizeof(chat_tx_buf) - i;

  cursor(true);
  cursor_pos(input_x,input_y);

  while(state == CHAT)
  {
    memset(chat_rx_buf,0,sizeof(chat_rx_buf));

    processKeyboard();
    rxFromNet();
  }
}
