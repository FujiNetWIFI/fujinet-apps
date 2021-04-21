#ifndef MENU_H
#define MENU_H

/* The various graphics modes, and the keypresses to
   choose them */
enum {
  CHOICE_NONE,
  CHOICE_HIRES_MONO,
  CHOICE_MEDRES_COLOR,
  CHOICE_LOWRES_GREY,
  CHOICE_LOWRES_RGB,
  CHOICE_MEDRES_RGB,
  NUM_CHOICES
};

extern unsigned char choice_keys[NUM_CHOICES];
extern char * modes[NUM_CHOICES];


/* The various sample options, and keys to select them */
enum {
  SAMPLE_NONE,
  SAMPLE_1,
  SAMPLE_2,
  SAMPLE_3,
  SAMPLE_4,
  SAMPLE_COLORBARS,
  NUM_SAMPLES
};

extern unsigned char sample_keys[NUM_SAMPLES];

/* Function prototypes: */

void dlist_setup_menu(void);
void show_chosen_date(unsigned char y, unsigned char m, unsigned char d, unsigned char loaded_properly);
void show_sample_choice(char sample);
void draw_menu(char sample, unsigned char y, unsigned char m, unsigned char d, unsigned char loaded_properly, unsigned char rgb_red, unsigned char rgb_grn, unsigned char rgb_blu);

#endif // MENU_H
