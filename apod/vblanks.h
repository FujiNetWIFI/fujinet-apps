#ifndef VBLANKS_H
#define VBLANKS_H

extern unsigned char dlist_hi, dlist_lo;
extern unsigned char rgb_ctr, apac_scanline, apac_lum;

void VBLANKD9(void);
void VBLANKD15(void);
void VBLANKD_APAC(void);

#endif // VBLANKS_H

