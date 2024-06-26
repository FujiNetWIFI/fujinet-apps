/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "../typedefs.h"

void input_init(void);
bool input_select_host(char *h);
SFState input_select_file_choose(void);
void input_line_filter(char *c);
void input_select_tape(void);
bool input_perform_confirm(void);
void input_perform_error();
PFState input_perform_done();

#endif /* INPUT_H */
