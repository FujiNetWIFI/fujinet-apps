/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Directory
 */
#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "state.h"

extern const char *directoryHeader;   /* Header text for page, from directory.c */
extern DirectoryState directoryState; /* current directory state */
extern char url[256];                 /* URL - minus currently selected prefix */
extern char line[80];                 /* re-usable line buffer */

void directory(void);

#endif /* DIRECTORY_H */
