/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef SELECT_FILE_H
#define SELECT_FILE_H

#include <stdbool.h>
#include "typedefs.h"

State select_file(void);
bool select_file_is_folder(void);
void select_file_display_long_filename(void);

#endif /* SELECT_FILE_H */
