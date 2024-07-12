#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <cc65.h>
#include <fujinet-network.h>

void vt100_init_terminal(void);
void vt100_exit_terminal(void);
void __fastcall__ vt100_process_inbound_char(uint8_t c);
void __fastcall__ vt100_process_outbound_char(uint8_t c);

extern void vt100_screen_cols;
extern void vt100_screen_rows;
#pragma zpsym ("vt100_screen_cols");
#pragma zpsym ("vt100_screen_rows");

#if defined(__ATARI__) || defined(__C64__)
int16_t get_key_if_available(void);
#endif

uint8_t device[256];
uint8_t buffer[1024];

void vt100_beep(void) {
#ifdef __APPLE2__
  uint8_t x = wherex();
#endif

  putchar('\a');

#ifdef __APPLE2__
  gotox(x);
#endif
}

void vt100_quit(void) {
  network_close(device);
}

void __fastcall__ vt100_send_char(uint8_t c) {
  network_write(device, &c, 1);
}

void __fastcall__ vt100_send_string(uint8_t *s) {
  network_write(device, s, strlen(s));
}

static bool connected(void)
{
  uint16_t bw;
  uint8_t c;
  uint8_t err;

  network_status(device, &bw, &c, &err);
  return c;
}

static void quit(void)
{
  if (doesclrscrafterexit()) {
    puts("Press any key to continue ...");
    cgetc();
  }
  exit(EXIT_FAILURE);
}

static void readline(uint8_t *s)
{
    uint16_t i = 0;
    uint8_t c;

    cursor(1);

    do {
#ifdef __APPLE2__
      gotox(i);
#endif

      c = cgetc();

      if (isprint(c)) {
        putchar(c);
        s[i++] = c;
      }
      else if ((c == CH_CURS_LEFT) || (c == CH_DEL)) {
        if (i) {
          putchar(CH_CURS_LEFT);
          putchar(' ');
          putchar(CH_CURS_LEFT);
          --i;
        }
      }
    } while (c != CH_ENTER);
    putchar('\n');
    s[i] = '\0';

    cursor(0);
}

void main(int argc, char *argv[])
{
  uint8_t cols = (uint8_t)&vt100_screen_cols;
  uint8_t rows = (uint8_t)&vt100_screen_rows;
  register int16_t retval;
  register uint8_t *bufptr;

  if (network_init() != FN_ERR_OK) {
    puts("Network init error");
    quit();
  }

  puts("----Example URLs-------------");
  puts("-                           -");
  puts("- TELNET://bbs.diller.org   -");
  puts("- SSH://user:password@host  -");
  puts("-                           -");
  puts("- use left arrow to delete  -");
  puts("-                           -");
  puts("-----------------------------");

  strcpy(device, "N:");
  if (argc == 2) {
    strcpy(device + 2, argv[1]);
  } else {
    puts("Enter SSH or Telnet URL:");
    readline(device + 2);
  }

  
  if (network_open(device, OPEN_MODE_RW, OPEN_TRANS_NONE) != FN_ERR_OK) {
    fputs("Network open error: ", stdout);
    puts(device + 2);
    quit();
  }
  
  vt100_init_terminal();

  while (connected()) {
    retval = network_read_nb(device, buffer, sizeof(buffer));
    if (retval > 0) {
      bufptr = buffer;
      while (retval--) {
        vt100_process_inbound_char(*bufptr++);
      }
    }

#if defined(__ATARI__) || defined(__C64__)
    retval = get_key_if_available();
    if (retval >= 0) {
      vt100_process_outbound_char(retval);
    }
#else
    if (kbhit()) {
      vt100_process_outbound_char(cgetc());
    }
#endif
  }

  vt100_exit_terminal();
}
