/**
 * @brief Text routines
 * @author Oliver Schmidt
 */

#ifndef TEXT_H
#define TEXT_H

void hires_putc(unsigned char x, unsigned char y, unsigned rop, unsigned char c);
void hires_putcc(unsigned char x, unsigned char y,unsigned rop, unsigned cc);
void hires_puts(unsigned char x, unsigned char y, unsigned rop, char *s);

#endif /* TEXT_H */
