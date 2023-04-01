/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Menu display routines
 */

#ifndef MENU_H
#define MENU_H

/* Direction keys */
#define LEFT 0x08
#define DOWN 0x0A
#define RIGHT 0x15
#define UP 0x0B
#define RETURN 0x0D
#define ESC 0x1B

/**
 * @brief Display menu item, highlighted if item=current item
 * @param ci Current menu item
 * @param i  menu item
 * @param x  column position of menu item (0-80)
 * @param y  row position of menu item (0-20)
 * @param s  The string to display
 */
void menu_item(unsigned char ci, unsigned char i, unsigned char x, unsigned char y, const char *s);

#endif /* MENU_H */
