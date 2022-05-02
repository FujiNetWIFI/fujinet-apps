/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef enum _sf_state
  {
    SF_INIT,
    SF_DISPLAY,
    SF_NEXT_PAGE,
    SF_PREV_PAGE,
    SF_CHOOSE,
    SF_FILTER,
    SF_ADVANCE_FOLDER,
    SF_DEVANCE_FOLDER,
    SF_DONE,
    SF_ABORT,
    SF_ERROR
  } SFState;

typedef enum _pf_state
  {
    PF_INIT,
    PF_CONFIRM,
    PF_RESTORE,
    PF_ABORT,
    PF_DONE,
    PF_ERROR,
    PF_RESTART
  } PFState;

typedef enum _state
  {
    SELECT_HOST,
    SELECT_FILE,
    SELECT_TAPE,
    PERFORM,
    DONE
  } State;

typedef char HostSlots[8][32];

typedef unsigned short DirectoryPosition;

#endif /* TYPEDEFS_H */
