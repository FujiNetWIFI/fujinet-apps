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
bool input_select_host(void);

#endif /* INPUT_H */
