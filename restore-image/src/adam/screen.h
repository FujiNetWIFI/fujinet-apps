/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>
#include "../typedefs.h"

void screen_init(void);
void screen_select_host(void);
void screen_bar(char y, char c, char m, char i, bool onOff);
void screen_select_file(void);
void screen_select_file_display(char *p, char *f,char *hsn);
void screen_select_file_display_entry(unsigned char y, char* e);
void screen_select_file_prev(void);
void screen_select_file_next(void);
void screen_select_file_display_long_filename(char *e);
void screen_select_file_clear_long_filename(void);
void screen_select_file_choose(char visibleEntries);
void screen_select_file_filter(void);
void screen_select_tape(void);
void screen_perform_confirm(void);
void screen_perform_read(unsigned long blockNum);
void screen_perform_write(unsigned long blockNum);
void screen_perform_error(void);
void screen_perform_verify_read(unsigned long blockNum);
void screen_perform_verify_check(void);
void screen_perform_abort(void);
void screen_perform_restore(void);
void screen_perform_done(void);

#endif /* SCREEN_H */
