#ifndef MENU_H
#define MENU_H

void dlist_setup_menu(void);
void show_chosen_date(unsigned char y, unsigned char m, unsigned char d, unsigned char loaded_properly);
void show_sample_choice(char sample);
void draw_menu(char sample, unsigned char y, unsigned char m, unsigned char d, unsigned char loaded_properly, unsigned char rgb_red, unsigned char rgb_grn, unsigned char rgb_blu);

#endif // MENU_H
