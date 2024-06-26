/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose Character set RAM segment ($1400)
 */

#pragma bss-name (push,"CHR")
unsigned char csram[2048];
#pragma bss-name (pop)
