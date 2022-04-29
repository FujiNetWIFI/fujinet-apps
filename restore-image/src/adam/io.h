/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef IO_H
#define IO_H

#include "../typedefs.h"

void io_init(void);
void io_select_host(void);
void io_close_directory(void);
void io_mount_host_slot(unsigned char hs);
void io_open_directory(unsigned char h, char *p, char *f);
void io_set_directory_position(DirectoryPosition pos);
char *io_read_directory(unsigned char l, unsigned char a);
void io_perform_open(char *path);

#endif /* IO_H */
