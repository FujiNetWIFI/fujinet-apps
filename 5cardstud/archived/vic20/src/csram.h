/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose Character set RAM segment ($1400)
 */

#ifndef CSRAM_H
#define CSRAM_H

#define CSROM_SIZE 2048

extern unsigned char csrom[CSROM_SIZE]; // in RODATA
extern unsigned char csram[CSROM_SIZE]; // in CHR

#endif /* CSRAM_H */
