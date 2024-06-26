/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose clock device functions
 */

#ifndef DT_H
#define DT_H

/**
 * @brief send formatted time string f to string s, derived from timestamp string t
 * @param s The destination string
 * @param f The strftime formatted string
 * @param t The timestamp value as string
 */
void dt(char *s, const char *f, const char *t);

#endif /* DT_H */
