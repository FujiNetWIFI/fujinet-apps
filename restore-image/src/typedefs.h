/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef enum _state
  {
    SELECT_HOST,
    SELECT_FILE,
    SELECT_TAPE,
    PERFORM,
    DONE
  } State;

typedef char HostSlots[8][32];

#endif /* TYPEDEFS_H */
