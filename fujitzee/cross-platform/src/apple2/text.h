#ifdef __APPLE2__

/**
 * @brief Text routines
 * @author Oliver Schmidt
 */

#ifndef TEXT_H
#define TEXT_H

void hires_putc(unsigned char x, unsigned char y, unsigned rop, unsigned char c);
void hires_putcc(unsigned char x, unsigned char y,unsigned rop, unsigned cc);
void hires_putcU(unsigned char x, unsigned char y, unsigned rop, unsigned char c);

void hires_mask(char xpos,    char ypos,
                char xsize,   char ysize,
                unsigned rop);

#endif /* TEXT_H */
#endif /* __APPLE2__ */