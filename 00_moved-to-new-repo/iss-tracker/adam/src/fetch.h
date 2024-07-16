/** 
 * iss-tracker
 *
 * based on Bill Kendrick's Atari version
 *
 * @author thomas cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details
 */

#ifndef FETCH_H
#define FETCH_H

#include "state.h"

State fetch(void);
void fetch_track(void);
void fetch_who(void);
unsigned char fetch_longitude_to_x(void);
unsigned char fetch_latitude_to_y(void);

#endif /* FETCH_H */
