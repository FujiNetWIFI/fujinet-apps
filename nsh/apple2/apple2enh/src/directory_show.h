/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Directory Show
 */

#ifndef DIRECTORY_SHOW_H
#define DIRECTORY_SHOW_H

#include <stdbool.h>

// bool directory_show(unsigned short pg, unsigned char *num_entries, unsigned bool *next_page);

bool directory_show(unsigned short pg, unsigned char *num_entries, bool *next_page);

#endif /* DIRECTORY_SHOW_H */
