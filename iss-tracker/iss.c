#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "nsio.h"
#include "faux_json.h"

#define VERSION "2021-05-18 \"OVERCAST\""
// #define DEBUG

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

/* Set up the screen & PMG */
void setup(void) {
  unsigned char i;

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
  for (i = 0; i < 79; i++) {
    POKE(dlist + 6 + i, DL_GRAPHICS7);
  }

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
  GTIA_WRITE.hposp0 = 0;
  OS.pcolr0 = 0x00;
  ANTIC.pmbase = ((unsigned int) pmg_mem) >> 8;

  /* Use our display list & enable the screen */
  OS.sdlst = dlist;
  OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH | DMACTL_DMA_PLAYERS;
  GTIA_WRITE.gractl = GRACTL_PLAYERS;
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

void blit_text(unsigned char x, unsigned char y, char * txt) {
  int i;
  unsigned int ybase;
  unsigned char yy, ch, pixels;
  unsigned char px1, px2, px3, px4, px5, px6, px7, px8;

  ybase = (unsigned int) (scr_mem + (y * 8 * 40));

  for (i = 0; txt[i] != '\0'; i++) {
    for (yy = 0; yy < 8; yy++) {
      ch = txt[i];

      if (ch < 32) {
        ch = ch + 64;
      } else if (ch < 96) {
        ch = ch - 32;
      }

      pixels = PEEK(CHARSET + (ch * 8) + yy);
      px1 = ((pixels & 0x80) >> 7) * 3;
      px2 = ((pixels & 0x40) >> 6) * 3;
      px3 = ((pixels & 0x20) >> 5) * 3;
      px4 = ((pixels & 0x10) >> 4) * 3;
      px5 = ((pixels & 0x08) >> 3) * 3;
      px6 = ((pixels & 0x04) >> 2) * 3;
      px7 = ((pixels & 0x02) >> 1) * 3;
      px8 = (pixels & 0x01) * 3;

      POKE(ybase + (yy * 40) + (x + i) * 2,
           (px1 << 6) | (px2 << 4) | (px3 << 2) | px4);
      POKE(ybase + (yy * 40) + (x + i) * 2 + 1,
           (px5 << 6) | (px6 << 4) | (px7 << 2) | px8);
    }
  }
}

void clear_message(void) {
  bzero(txt_mem, 160);
}

void message(int x, int y, char * txt) {
  int i;
  unsigned char ch;
  unsigned int ybase;

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

void fetch_json(char * url) {
  int data_len;

  nopen(1, url, 4 /* read */);
  nstatus(1);
  data_len = (OS.dvstat[1] << 8) + OS.dvstat[0];

  nread(1, json, data_len);
  nclose(1);
}

unsigned char txt[80];

void main(void) {
  int i, lat, lon;
  unsigned char n, x, y, key;

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

  /* Set the proper map colors (from the image file) */
  OS.color4 = map_data[3200];
  OS.color0 = map_data[3201];
  OS.color1 = map_data[3202];
  OS.color2 = map_data[3203];


  /* Draw title and credits */
  blit_text(0, 0, "International Space");
  blit_text(0, 1, "Station Tracker for");
  blit_text(6, 2, "#FujiNet");

  blit_text(1, 4, "Bill Kendrick, 2021");

  blit_text(2, 6, "Using data from");
  blit_text(2, 7, "Nathan Bergey's");
  blit_text(1, 8, "\"Open Notify\" APIs");

  x = 16 - strlen(VERSION) / 2;
  message(x, 1, "Version ");
  message(x + 8, 1, VERSION);

  message(6, 3, "Press any key to continue...");

  OS.ch = KEY_NONE;
  do {
  } while (OS.ch == KEY_NONE);

  clear_message();
  memcpy((unsigned char *) scr_mem, (unsigned char *) map_data, 3200);

  /* MAIN LOOP */
  OS.ch = KEY_NONE;
  do {
    /* Fetch the ISS's current position */
    clear_message();
    message(8, 1, "Fetching ISS position...");

#ifdef DEBUG
  strcpy(json,
    "{\"timestamp\": 1621328742, \"iss_position\": {\"longitude\": \"-49.9018\", \"latitude\": \"-15.4486\"}, \"message\": \"success\"}"
  );
#else
    fetch_json("N:HTTP://api.open-notify.org/iss-now.json");
#endif

    clear_message();

    if (json[0] == '\0') {
      /* ERROR */
      message(6, 0, "Cannot read from open-notify!");
      message(8, 1, "Press a key to retry...");
      GTIA_WRITE.hposp0 = 0; /* move PMG off the screen */

      OS.ch = KEY_NONE;
      do {
      } while (OS.ch == KEY_NONE);
      OS.ch = KEY_NONE;
    } else {
      /* SUCCESS */

      /* Parse and display position */
      faux_parse_json("latitude\": \"", 0);
      lat = atoi(json_part);

      message(0, 0, "Latitude: ");
      message(10, 0, json_part);

      faux_parse_json("longitude\": \"", 0);
      lon = atoi(json_part);

      message(20, 0, "Longitude: ");
      message(31, 0, json_part);

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
         so we want that to be the /center/ of where we draw the sprite */
      bzero(pmg_mem + 512, 128);
      memcpy(pmg_mem + 512 + 16 - (SPRITE_HEIGHT / 2) + y, sprite, SPRITE_HEIGHT);

      /* Left of screen is at PMG horizontal position 48 */
      GTIA_WRITE.hposp0 = 48 - (SPRITE_WIDTH / 2) + x;

      message(3, 2, "Press [P] to see who is in space!");
      message(3, 3, "Press any other key to refresh...");

      OS.ch = KEY_NONE;
      do {
        OS.pcolr0 = ((OS.rtclok[2] >> 1) & 0x0F) + 16;
        key = OS.ch;
      } while (key == KEY_NONE);
      OS.ch = KEY_NONE;

      if (key == KEY_P) {
        /* FIXME */

        /* Fetch list of who is in space */
        clear_message();
        message(2, 1, "Fetching who is in space position...");

#ifdef DEBUG
        strcpy(json,
          "{\"number\": 7, \"message\": \"success\", \"people\": [{\"name\": \"Mark Vande Hei\", \"craft\": \"ISS\"}, {\"name\": \"Oleg Novitskiy\", \"craft\": \"ISS\"}, {\"name\": \"Pyotr Dubrov\", \"craft\": \"ISS\"}, {\"name\": \"Thomas Pesquet\", \"craft\": \"ISS\"}, {\"name\": \"Megan McArthur\", \"craft\": \"ISS\"}, {\"name\": \"Shane Kimbrough\", \"craft\": \"ISS\"}, {\"name\": \"Akihiko Hoshide\", \"craft\": \"ISS\"}]}"
        );
#else
        fetch_json("N:HTTP://api.open-notify.org/astros.json");
#endif

        clear_message();

        if (json[0] == '\0') {
          /* ERROR */
          message(6, 0, "Cannot read from open-notify!");
          message(7, 1, "Press a key to continue...");
    
          OS.ch = KEY_NONE;
          do {
          } while (OS.ch == KEY_NONE);
          OS.ch = KEY_NONE;
        } else {
          /* SUCCESS */

          faux_parse_json("number\": ", 0);
          
          blit_text(0, 3, "There are    people");
          blit_text(10, 3, json_part);
          blit_text(0, 4, "in space right now!");

          n = atoi(json_part);

          key = KEY_NONE;
          for (i = 0; i < n && key != KEY_ESC; i++) {
            faux_parse_json("\"name\": ", i);
            blit_text(0, 6, "                    ");
            blit_text(10 - strlen(json_part) / 2, 6, json_part);

            clear_message();
            snprintf(txt, 80, "#%d: %s", i + 1, json_part);
            message(0, 0, txt);

            faux_parse_json("\"craft\": ", i);
            snprintf(txt, 80, "is on %s", json_part);
            message(0, 1, txt);

            message(7, 2, "Press a key to continue...");
            if (i < n - 1) {
              message(5, 3, "(or [Esc] to return to the map)");
            }

            OS.ch = KEY_NONE;
            do {
              key = OS.ch;
            } while (key == KEY_NONE);
            OS.ch = KEY_NONE;
            clear_message();
          }

          /* Put map back: */
          memcpy((unsigned char *) scr_mem, (unsigned char *) map_data, 3200);

          key = KEY_NONE; /* Don't let [Esc] here fall thru to the main loop! */
        }
      }
    }
  } while (key != KEY_ESC);

  asm("jmp $E477"); /* Cold start */

  return;
}
