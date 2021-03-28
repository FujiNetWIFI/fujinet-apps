/*
  apod.c

  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  See the APOD web app (server)

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-03-27
*/

#include <stdio.h>
#include <atari.h>
#include <peekpoke.h>

/**
 * Simple text rendering onto screen memory
 */
void myprint(unsigned char x, unsigned char y, char * str) {
  int pos, i, SC;
  unsigned char c;

  SC = PEEKW(88);

  pos = y * 20 + x;
  for (i = 0; str[i] != '\0'; i++) {
    c = str[i];

    if (c < 32) {
      c = c + 64;
    } else if (c < 96) {
      c = c - 32;
    }

    POKE(SC + pos + i, c);
  }
}

/* The various graphics modes, the keypresses to
   choose them, and the argument to send to the
   web app via "?mode=". */
enum {
  CHOICE_NONE,
  CHOICE_HIRES_MONO,
  CHOICE_MEDRES_GREY,
  CHOICE_LOWRES_GREY,
  NUM_CHOICES
};

unsigned char choice_keys[NUM_CHOICES] = {
  KEY_NONE,
  KEY_A,
  KEY_B,
  KEY_C
};

char * modes[NUM_CHOICES] = {
  "",
  "8",
  "15",
  "9"
};


/* The base URL for the web app */
char * default_baseurl = "N:HTTP://billsgames.com/fujinet/apod/index.php";
char * baseurl;

/* Space to store the composed URL */
char url[255];


/* The program! */
void main(void) {
  FILE * fi;
  unsigned char keypress, choice;
  int i;

  do {
    if (0) {
      /* FIXME Accept command-line options */
      choice = CHOICE_HIRES_MONO;
      baseurl = default_baseurl;
    } else {
      /* Prompt user for the preferred viewing mode */
      _graphics(2+16);
      myprint(0, 0, "Astronomy Picture Of");
      myprint(3, 1, "the Day (APOD)");
      myprint(4, 2, "via #FUJINET");
      myprint(1, 3, "bill kendrick 2021");
  
      myprint(0, 5, "A high res mono");
      myprint(0, 6, "B medium res 4 shade");
      myprint(0, 7, "C low res 16 shade");
  
      baseurl = default_baseurl;
  
      /* Accept a choice */
      choice = CHOICE_NONE;
      OS.ch = KEY_NONE;
      do {
        while (OS.ch == KEY_NONE) { OS.color0 = (OS.rtclok[2] >> 2) << 2; }
        keypress = OS.ch;
        OS.ch = KEY_NONE;
  
        for (i = 0; i < NUM_CHOICES; i++) {
          if (choice_keys[i] == keypress) {
            choice = i;
          }
        }
      } while (choice == CHOICE_NONE);
    }
  
    /* Set up the display, based on the choice */
    if (choice == CHOICE_HIRES_MONO || CHOICE_LOWRES_GREY) {
      _graphics(8);
      OS.color1 = 15;
      OS.color2 = 0;
      OS.color4 = 2;
  
      OS.lmargn = 0;
      OS.rmargn = 39;
      OS.crsinh = 1;
      putchar(CH_CLR);
  
      if (choice == CHOICE_LOWRES_GREY) {
        /* FIXME: Set up a DLI to keep text window readable */
        OS.gprior = 64;
      }
    } else if (choice == CHOICE_MEDRES_GREY) {
      _graphics(15);
  
      OS.color4 = 0;
      OS.color1 = 4;
      OS.color2 = 8;
      OS.color3 = 14;
  
      OS.lmargn = 0;
      OS.rmargn = 39;
      OS.crsinh = 1;
      putchar(CH_CLR);
    } else if (choice == CHOICE_LOWRES_GREY) {
      _graphics(9);
      OS.color4 = 16;
    }
  
    /* Load the data! */
    snprintf(url, sizeof(url), "%s?mode=%s", baseurl, modes[choice]);
  
    fi = fopen(default_baseurl, "rb");
    if (fi == NULL) {
      printf("Oops %s", url);
    } else {
      fread((void *) PEEKW(88), (size_t) 7680, (size_t) 1, fi);
      fclose(fi);
    }
  
    OS.ch = KEY_NONE;
    do { } while(OS.ch == KEY_NONE);
    OS.ch = KEY_NONE;
  } while(1);
}
