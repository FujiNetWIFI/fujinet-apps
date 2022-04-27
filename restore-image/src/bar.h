/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef BAR_H
#define BAR_H

void bar_init(unsigned char y, unsigned char c, unsigned char m, unsigned char i);
void bar_up(void);
void bar_down(void);
char bar_get(void);
void bar_set(char i);
void bar_done(void);

#endif /* BAR_H */
