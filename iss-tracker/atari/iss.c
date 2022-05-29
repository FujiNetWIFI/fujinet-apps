#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "nsio.h"
#include "colors.h"

#define VERSION "2021-05-29 \"ARGONAUTS\"" /* get it? "JSON and the..."? */

/* How long to wait before auto-refresh */
#define RTCLOK1_WAIT ((3 /* minutes */ * 60 /* seconds per minute */ * 60 /* 'jiffies' per second */) / 256 /* RTCLOK2 cycles per RTCLOK1 increment */)

/* We fetch 10 x 2 positions per request = 20 positions.
   At 5 minutes, we get 20 * 5 = 100 minutes, or approx. 1.5 hrs
   of future positions (approx. one orbit!) */
#define TRACK_TIMESKIP (5 /* minutes */ * 60 /* seconds per minute */)

/* A chunk of memory for the screen
   (bitmapped graphics, text window, and display list),
   as described in `atari.cfg`, and found in `segments.s` */
extern unsigned char scr_mem[];

/* Pointers within `scr_mem` */
unsigned char * dlist;
unsigned char * txt_mem;

/* A chunk of memory for the Player/Missile Graphics */
extern unsigned char pmg_mem[];

/* The world map bitmap */
extern unsigned char map_data[];

unsigned char old_x[4], old_y[4];

#define BELL_ABORT 0
#define BELL_GOOD 1

void bell(unsigned char bell_type) {
  unsigned char i;

  POKEY_WRITE.audf2 = 100 - (50 * bell_type);
  for (i = 0; i < 255; i++) {
    POKEY_WRITE.audc2 = 0xAF - (i >> 4);
  }
}

void dli(void) {
  asm("pha");

  asm("lda #$90");
  asm("sta $D40A"); // WSYNC
  asm("sta $D018"); // COLPF2

  asm("pla");
  asm("rti");
}


/* Set up the screen & PMG */
void setup(void) {
  unsigned char i;

  POKEY_WRITE.audctl = 0;

  POKEY_WRITE.audf1 = 0;
  POKEY_WRITE.audc1 = 0;
  POKEY_WRITE.audf2 = 0;
  POKEY_WRITE.audc2 = 0;
  POKEY_WRITE.audf3 = 0;
  POKEY_WRITE.audc3 = 0;
  POKEY_WRITE.audf4 = 0;
  POKEY_WRITE.audc4 = 0;

  POKEY_WRITE.skctl = SKCTL_KEYBOARD_DEBOUNCE | SKCTL_KEYBOARD_SCANNING;

  /* Turn off the screen (ANTIC DMA) */
  OS.sdmctl = 0;

  /* Set our pointers */
  txt_mem = scr_mem + (40 * 80);
  dlist = txt_mem + (40 * 4);

  /* Clear everything */
  bzero(scr_mem, (dlist - scr_mem + 1));
  bzero(pmg_mem, 1024);

  /* Generate a display list... */

  /* Black border at the top */
  POKE(dlist, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK8);

  /* First line of bitmap graphics, with LMS */
  POKE(dlist + 3, DL_LMS(DL_GRAPHICS7));
  POKEW(dlist + 4, (unsigned int) scr_mem);

  /* 79 more lines of graphics */
  for (i = 0; i < 78; i++) {
    POKE(dlist + 6 + i, DL_GRAPHICS7);
  }
  POKE(dlist + 6 + 78, DL_DLI(DL_GRAPHICS7));

  /* First line of text window, with LMS */
  POKE(dlist + 85, DL_LMS(DL_GRAPHICS0));
  POKEW(dlist + 86, (unsigned int) txt_mem);
  for (i = 0; i < 3; i++) {
    POKE(dlist + 88 + i, DL_GRAPHICS0);
  }

  POKE(dlist + 91, DL_JVB);
  POKEW(dlist + 92, (unsigned int) dlist);

  /* Config PMGs */

  OS.gprior = PRIOR_P03_PF03;
  GTIA_WRITE.sizep0 = PMG_SIZE_NORMAL;
  GTIA_WRITE.sizep1 = PMG_SIZE_NORMAL;
  GTIA_WRITE.sizep2 = PMG_SIZE_NORMAL;
  GTIA_WRITE.sizep3 = PMG_SIZE_NORMAL;
  GTIA_WRITE.hposp0 = 0;
  GTIA_WRITE.hposp1 = 0;
  GTIA_WRITE.hposp2 = 0;
  GTIA_WRITE.hposp3 = 0;
  OS.pcolr0 = 0x00;
  OS.pcolr1 = 0x04;
  OS.pcolr2 = 0x08;
  OS.pcolr3 = 0x0C;
  ANTIC.pmbase = ((unsigned int) pmg_mem) >> 8;

  bzero(old_x, 4);
  bzero(old_y, 4);

  /* Use our display list & enable the screen */
  OS.sdlst = dlist;
  OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH | DMACTL_DMA_PLAYERS;
  GTIA_WRITE.gractl = GRACTL_PLAYERS;

  ANTIC.nmien = NMIEN_VBI;
  while (ANTIC.vcount < 124);
  OS.vdslst = (void *) dli;
  ANTIC.nmien = NMIEN_VBI | NMIEN_DLI;

  /* Set default colors, then try to load from an app key */
  set_default_colors();
}

#define X_CENTER 79
#define Y_CENTER 39

#define SPRITE_HEIGHT 8
#define SPRITE_WIDTH 8 /* Players are 8 pixels wide! */

unsigned char sprite[SPRITE_HEIGHT] = {
  0x20, /* ..X.|.... */
  0x50, /* .X.X|.... */
  0xA4, /* X.X.|.X.. */
  0x58, /* .X.X|X... */
  0x1A, /* ...X|X.X. */
  0x05, /* ....|.X.X */
  0x0A, /* ....|X.X. */
  0x04, /* ....|.X.. */
};

#define CHARSET 0xE000

unsigned char pxcolor[2] = { 2, 0 };

void blit_text(unsigned char x, unsigned char y, char * txt) {
  int i;
  unsigned int ybase, dest;
  unsigned char yy, ch, pixels;
  unsigned char px1, px2, px3, px4, px5, px6, px7, px8;

  ybase = (unsigned int) (scr_mem + ((y << 3) * 40));

  for (i = 0; txt[i] != '\0'; i++) {
    ch = txt[i];

    if (ch < 32) {
      ch = ch + 64;
    } else if (ch < 96) {
      ch = ch - 32;
    }

    for (yy = 0; yy < 8; yy++) {
      pixels = PEEK(CHARSET + (ch << 3) + yy);
      px1 = pxcolor[((pixels & 0x80) >> 7)];
      px2 = pxcolor[((pixels & 0x40) >> 6)];
      px3 = pxcolor[((pixels & 0x20) >> 5)];
      px4 = pxcolor[((pixels & 0x10) >> 4)];
      px5 = pxcolor[((pixels & 0x08) >> 3)];
      px6 = pxcolor[((pixels & 0x04) >> 2)];
      px7 = pxcolor[((pixels & 0x02) >> 1)];
      px8 = pxcolor[(pixels & 0x01)];

      dest = ybase + (yy * 40) + ((x + i) << 1);
      POKE(dest, (px1 << 6) | (px2 << 4) | (px3 << 2) | px4);
      POKE(dest + 1, (px5 << 6) | (px6 << 4) | (px7 << 2) | px8);
    }
  }
}

void clear_message(void) {
  bzero(txt_mem, 160);
}

#define MSG_CENTER 99

void message(int x, int y, char * txt) {
  int i;
  unsigned char ch;
  unsigned int ybase;

  if (x == MSG_CENTER) {
    x = (20 - (strlen(txt) >> 1));
  }

  ybase = (unsigned int) (txt_mem + (y * 40));

  for (i = 0; txt[i] != '\0'; i++) {
    ch = txt[i];

    if (ch < 32) {
      ch = ch + 64;
    } else if (ch < 96) {
      ch = ch - 32;
    }

    POKE(ybase + x + i, ch);
  }
}

unsigned char txt[80], url[128];

unsigned char flash_colors[16] = {
  0x10, 0x12, 0x24, 0x26, 0x38, 0x3A, 0x3C, 0x4E,
  0x3C, 0x3A, 0x38, 0x26, 0x84, 0x12, 0x10, 0x00
};


unsigned char open_json(unsigned char * url) {
  unsigned char err;

  err = nopen(1, url, 12);
  if (err != 1)
    return err;
  
  err = nchanmode(1, 12, CHANNELMODE_JSON);
  if (err != 1)
    return err;

  err = njsonparse(1, 12);
  return err;
}


unsigned char json_part[256];
unsigned char query[256];
unsigned char tmp[256];

void parse_json(unsigned char * element) {
  unsigned char err;
  int data_len;

  json_part[0] = '\0';

  sprintf(query, "N1:%s%c", element, CH_EOL);
  err = njsonquery(1, 12, (char *) query);
  /* FIXME: Detect error */

  err = nstatus(1);
  /* FIXME: Detect error */

  data_len = (OS.dvstat[1] << 8) + OS.dvstat[0];
  if (data_len == 0) {
    return; /* FIXME: Error! */
  }

  err = nread(1, json_part, data_len);
  json_part[data_len - 1 /* eat final char because of "ending in an ATASCII EOL" */] = '\0';
  /* FIXME: Detect error */
}


void main(void) {
  int i, j, lat, lon, last_space;
  unsigned char n, x, y, key, done, err;
  long timestamp;
  char * ptr;

  /* Set up the screen */
  setup();

  /* Display the map (simple animation effect) */
  OS.color4 = 2;
  OS.color0 = 4;
  OS.color1 = 6;
  OS.color2 = 8;

  for (i = 0; i < 40; i++) {
    memcpy((unsigned char *) scr_mem + (39 - i) * 40, (unsigned char *) map_data + (39 - i) * 40, 40);
    memcpy((unsigned char *) scr_mem + (i + 40) * 40, (unsigned char *) map_data + (i + 40) * 40, 40);
    n = OS.rtclok[2] + 1;
    do { } while (OS.rtclok[2] != n);
  }

  /* Set the final map colors */
  OS.color4 = color4;
  OS.color0 = color0;
  OS.color1 = color1;
  OS.color2 = color2;


  /* Draw title and credits */
  blit_text(0, 0, "International Space");
  blit_text(0, 1, "Station Tracker for");
  blit_text(6, 2, "#FujiNet");

  blit_text(1, 3, "Bill Kendrick, 2021");

  blit_text(2, 5, "Using data from");
  blit_text(2, 6, "Nathan Bergey's");
  blit_text(3, 7, "\"Open Notify\"");
  blit_text(2, 8, "and Bill Shupp's");
  blit_text(1, 9, "\"Where The ISS At?\"");

  x = 16 - strlen(VERSION) / 2;
  message(x, 1, "Version ");
  message(x + 8, 1, VERSION);

  message(MSG_CENTER, 3, "Press any key to continue...");

  OS.ch = KEY_NONE;
  do {
  } while (OS.ch == KEY_NONE);

  bell(BELL_GOOD);

  clear_message();
  memcpy((unsigned char *) scr_mem, (unsigned char *) map_data, 3200);

  /* MAIN LOOP */
  OS.ch = KEY_NONE;
  do {
    /* Fetch the ISS's current position */
    clear_message();
    message(MSG_CENTER, 1, "Fetching ISS position...");

    err = open_json("N:HTTP://api.open-notify.org/iss-now.json");
    /*
      {"iss_position": {"longitude": "60.0548", "latitude": "7.1665"}, "message": "success", "timestamp": 1653813276}
    */

    clear_message();

    if (err != 1) {
      /* ERROR */
      message(MSG_CENTER, 0, "Cannot read from open-notify!");
      message(MSG_CENTER, 1, "Press a key to retry...");
      GTIA_WRITE.hposp0 = 0; /* move PMG off the screen */

      OS.ch = KEY_NONE;
      do {
      } while (OS.ch == KEY_NONE);
      OS.ch = KEY_NONE;
    } else {
      /* SUCCESS */

      /* Parse and display position */
      parse_json("/iss_position/latitude");
      lat = atoi(json_part);

      message(0, 0, "Latitude: ");
      json_part[7] = '\0';
      message(10, 0, json_part);
      message(10 + strlen(json_part), 0, "N");

      parse_json("/iss_position/longitude");
      lon = atoi(json_part);

      message(20, 0, "Longitude: ");
      json_part[7] = '\0';
      message(31, 0, json_part);
      message(31 + strlen(json_part), 0, "E");

      parse_json("/timestamp");
      timestamp = atol(json_part);

      nchanmode(1, 12, CHANNELMODE_PROTOCOL);
      nclose(1);

      /* Draw the ISS in its position over the map */

      /* Map longitude (-180 -> 180 degrees east) to screen X position (0 left -> 159 right) */
      x = X_CENTER + (unsigned char) ((lon << 2) / 9);

      /* Map latitude (-90 -> 90 degrees north) to screen Y position (0 top -> 79 bottom) */
      y = Y_CENTER - (unsigned char) ((lat << 2) / 9);

      /* Note: Both calculations are (N / 2.25):
         * -180 -> 180 is 360 degrees, mapped to 160 horizontal positions,
         * -90 -> 90 is 180 degress, mapped to 80 vertical positions
         This is being done in integer math as ((N * 4) / 9)
      */

      /* Top of screen is at PMG vertical position 16,
         so we want that to be the /center/ of where we draw the sprite.
         Left of screen is at PMG horizontal position 48 */

      bzero(pmg_mem + 512, 512);

      old_x[3] = old_x[2];
      old_y[3] = old_y[2];
      old_x[2] = old_x[1];
      old_y[2] = old_y[1];
      old_x[1] = old_x[0];
      old_y[1] = old_y[0];
      old_x[0] = 48 - (SPRITE_WIDTH >> 1) + x;
      old_y[0] = 16 - (SPRITE_HEIGHT >> 1) + y;

      for (i = 0; i < 4; i++) {
        memcpy(pmg_mem + 512 + (i * 128) + old_y[i], sprite, SPRITE_HEIGHT);
        *(&(GTIA_WRITE.hposp0) + i) = old_x[i];
      }

      message(MSG_CENTER, 1, "Press [P] to see who is in space!");
      message(MSG_CENTER, 2, "Press [R] to refresh, [T] to track.");
      // message(MSG_CENTER, 3, "Press [C] to change colors.");

      OS.rtclok[1] = 0;
      OS.rtclok[2] = 0;

      OS.ch = KEY_NONE;
      do {
        OS.pcolr0 = flash_colors[(OS.rtclok[2] >> 2) & 0x0F];
        key = OS.ch;
      } while (
        key != KEY_P &&
        key != KEY_R &&
        key != KEY_T &&
        key != KEY_ESC &&
        OS.rtclok[1] < RTCLOK1_WAIT
      );
      OS.ch = KEY_NONE;

      if (key == KEY_P) {
        bell(BELL_GOOD);

        /* Fetch list of who is in space */
        clear_message();
        message(MSG_CENTER, 1, "Fetching who is in space position...");

        err = open_json("N:HTTP://api.open-notify.org/astros.json");
        /*
        {
          "people": [
            {"craft": "ISS", "name": "Oleg Artemyev"},
            {"craft": "ISS", "name": "Denis Matveev"},
            ...
          ],
          "message": "success",
          "number": 7
        }
        */

        clear_message();

        if (err != 1) {
          /* ERROR */
          message(MSG_CENTER, 0, "Cannot read from open-notify!");
          message(MSG_CENTER, 1, "Press a key to continue...");
    
          OS.ch = KEY_NONE;
          do {
          } while (OS.ch == KEY_NONE);
          OS.ch = KEY_NONE;
        } else {
          /* SUCCESS */

          GTIA_WRITE.hposp0 = 0;
          GTIA_WRITE.hposp1 = 0;
          GTIA_WRITE.hposp2 = 0;
          GTIA_WRITE.hposp3 = 0;
 
          parse_json("/number");
          
          blit_text(0, 0, "There are    people");
          blit_text(10, 0, json_part);
          blit_text(0, 1, "in space right now!");

          n = atoi(json_part);

          key = KEY_NONE;
          for (i = 0; i < n && key != KEY_ESC; i++) {
            sprintf(tmp, "/people/%d/name", i);
            parse_json(tmp);

            y = 6 - (strlen(json_part) / 40);
            ptr = json_part;
            done = 0;
            while (!done) {
              strcpy(txt, ptr);
              if (strlen(txt) < 20) {
                done = 1;
              } else {
                last_space = 0;
                for (j = 0; j < 20 && j < strlen(txt); j++) {
                  if (txt[j] == ' ') {
                    last_space = j;
                  }
                }
                if (last_space == 0) {
                  last_space = 20;
                }
                txt[last_space] = '\0';
                ptr += last_space;
              }
              memset(scr_mem + (y * 8) * 40, pxcolor[0] << 6 | pxcolor[0] << 4 | pxcolor[0] << 2 | pxcolor[0], 8 * 40);
              blit_text(10 - strlen(txt) / 2, y, txt);
              y++;
            }

            clear_message();
            snprintf(txt, 80, "%s is on ", json_part);

            sprintf(tmp, "/people/%d/craft", i);
            parse_json(tmp);
            strcat(txt, json_part);

            strcpy(json_part, txt);

            y = 0;
            ptr = json_part;
            done = 0;
            while (!done) {
              strcpy(txt, ptr);
              if (strlen(txt) < 40) {
                done = 1;
              } else {
                last_space = 0;
                for (j = 0; j < 40 && j < strlen(txt); j++) {
                  if (txt[j] == ' ') {
                    last_space = j;
                  }
                }
                if (last_space == 0) {
                  last_space = 40;
                }
                txt[last_space] = '\0';
                ptr += last_space;
              }
              message(MSG_CENTER, y, txt);
              y++;
            }

            if (i < n - 1) {
              sprintf(txt, "(%d) Press a key for more ([Esc] = stop)", i + 1);
            } else {
              sprintf(txt, "(%d) Press a key", i + 1);
            }
            message(MSG_CENTER, 3, txt);

            OS.ch = KEY_NONE;
            do {
              key = OS.ch;
            } while (key == KEY_NONE);
            OS.ch = KEY_NONE;
            clear_message();

            if (key != KEY_ESC) {
              bell(BELL_GOOD);
              memcpy(scr_mem + 640, map_data + 640, 2560);
            } else {
              bell(BELL_ABORT);
            }
          }

          nchanmode(1, 12, CHANNELMODE_PROTOCOL);
          nclose(1);

          /* Clear map: */
          memcpy((unsigned char *) scr_mem, (unsigned char *) map_data, 3200);

          key = KEY_NONE; /* Don't let [Esc] here fall thru to the main loop! */
        }
      } else if (key == KEY_T) {
        bell(BELL_GOOD);

        /* Clear map: */
        memcpy((unsigned char *) scr_mem, (unsigned char *) map_data, 3200);

        /* Fetch some upcoming ISS positions based (via timestamps) */
        clear_message();
        message(MSG_CENTER, 1, "Fetching upcoming ISS positions...");
        message(MSG_CENTER, 3, "(Press [Esc] to abort)");

        for (i = 0; i < 10 && OS.ch != KEY_ESC; i++) {
          sprintf(txt, "%d..", i + 1);
          message(i * 4, 2, txt);

          /* 25544 is the NORAD catalog id for ISS */
          sprintf(url, "N:HTTP://api.wheretheiss.at/v1/satellites/25544/positions?timestamps=%ld,%ld",
                  timestamp + TRACK_TIMESKIP * (i * 2),
                  timestamp + TRACK_TIMESKIP * ((i * 2) + 1));
          open_json(url);

          for (j = 0; j < 2; j++) {
            parse_json("latitude\":"); /* FIXME */
            lat = atoi(json_part);
  
            parse_json("longitude\":"); /* FIXME */
            lon = atoi(json_part);
  
            /* Map longitude (-180 -> 180 degrees east) to screen X position (0 left -> 159 right) */
            x = X_CENTER + (unsigned char) ((lon << 2) / 9);
  
            /* Map latitude (-90 -> 90 degrees north) to screen Y position (0 top -> 79 bottom) */
            y = Y_CENTER - (unsigned char) ((lat << 2) / 9);
  
            /* FIXME: A better plot routine would be good here! */
            scr_mem[y * 40 + (x >> 2)] = 0x00;
          }

          /* Pause 1 second (API rate-limit requirement!) */
          OS.rtclok[2] = 0;
          do {
          } while (OS.rtclok[2] < 60 && OS.ch != KEY_ESC);
        }
        if (OS.ch == KEY_ESC) {
          bell(BELL_ABORT);
        }
        OS.ch = KEY_NONE;

        key = KEY_NONE; /* Don't let [Esc] here fall thru to the main loop! */
      } else if (key == KEY_R) {
        bell(BELL_GOOD);
      }
    }
  } while (key != KEY_ESC);

  asm("jmp $E477"); /* Cold start */

  return;
}
