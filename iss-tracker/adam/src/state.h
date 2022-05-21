/** 
 * iss-tracker
 *
 * based on Bill Kendrick's Atari version
 *
 * @author thomas cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details
 */

#ifndef STATE_H
#define STATE_H

typedef enum _state
  {
    INIT,
    FETCH,
    DISPLAY,
    TRACK,
    WHO
  } State;

#endif /* STATE_H */
