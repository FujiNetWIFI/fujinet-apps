/**
 * @brief   Wrapper for KERNAL video routines
 * @author  Thom Cherryhomes
 * @email   thom cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details.
 */

#ifndef VIDEO_H
#define VIDEO_H

extern void clrscr();

/**
 * @brief set up charset
 */
void video_setup(void);

/**
 * @brief simple screen print line
 * @param s NULL terminated string to print.
 */
void print(const char *s);

#endif /* VIDEO_H */
