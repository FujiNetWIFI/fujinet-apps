/**
 * @brief Text routines
 * @author Oliver Schmidt
 */

#ifndef TEXT_H
#define TEXT_H

void hires_putc(char x, char y, unsigned rop, char c);
void hires_puts(char x, char y, unsigned rop, char *s);
void status(char *s);

#endif /* TEXT_H */
