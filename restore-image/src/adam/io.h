/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include "../typedefs.h"

void io_init(void);
void io_select_host(void);
void io_close_directory(void);
void io_mount_host_slot(unsigned char hs);
void io_open_directory(unsigned char h, char *p, char *f);
void io_set_directory_position(DirectoryPosition pos);
char *io_read_directory(unsigned char l, unsigned char a);
void io_perform_open(char *path);
void io_perform_close(void);
bool io_perform_read_block(unsigned long blockNum, unsigned char *buf);
unsigned char io_perform_eom(void);
unsigned char io_perform_write_error(void);
unsigned char io_perform_write_block(unsigned char dev, unsigned long blockNum, unsigned char *buf);
unsigned char io_perform_verify_block(unsigned char dev, unsigned long blockNum, unsigned char *buf);
void io_perform_rewind(void);

#endif /* IO_H */
