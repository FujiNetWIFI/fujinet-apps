#ifndef MENU_H
#define MENU_H

/* The various graphics modes */
enum {
  /* No choice made (yet) */
  CHOICE_NONE,

  /* Hi-Res mono (320x192 black & white; GRAPHICS 8) */
  CHOICE_HIRES_MONO,

  /* Med-Res 4 color (160x192 4 color (chosen by server); GRAPHICS 15 aka GRAPHICS 7+) */
  CHOICE_MEDRES_COLOR,

  /* Lo-Res 16 shade (80x192 greyscale; GRAPHICS 9) */
  CHOICE_LOWRES_GREY,

  /* Lo-Res 4096 color (80x192 4096 color via Red/Green/Blue; ColorView9) */
  CHOICE_LOWRES_RGB,

  /* Med-Res 64 color (160x192 64 color via Red/Green/Blue; ColorView15) */
  CHOICE_MEDRES_RGB,

  /* Lo-Res 256 color (80x192 256 color via Hue/Lum; APAC) */
  CHOICE_LOWRES_256,

  NUM_CHOICES
};

extern unsigned char choice_keys[NUM_CHOICES];
extern char * modes[NUM_CHOICES];


/* The various sample options */
enum {
  /* Don't fetch a sample; fetch APOD photo (current, or for chosen date) */
  SAMPLE_NONE,

  /* Fetch Samples 1...5 (see APOD server) */
  SAMPLE_1,
  SAMPLE_2,
  SAMPLE_3,
  SAMPLE_4,
  SAMPLE_5,

  /* Render (locally) a test pattern (e.g., color bars) */
  SAMPLE_COLORBARS,

  NUM_SAMPLES
};

extern unsigned char sample_keys[NUM_SAMPLES];

void dlist_setup_menu(void);
void show_chosen_date(unsigned char y, unsigned char m, unsigned char d, unsigned char loaded_properly);
void show_sample_choice(char sample);
void draw_menu(char sample, unsigned char y, unsigned char m, unsigned char d, unsigned char loaded_properly);
void handle_menu(unsigned char * choice, char * sample);

#endif // MENU_H
