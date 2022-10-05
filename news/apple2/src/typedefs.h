/**
 * #FUJINET News
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#define KEY_RETURN       0x0D
#define KEY_ESCAPE       0x1B
#define KEY_SPACE        0x20
#define KEY_1            0x31
#define KEY_2            0x32
#define KEY_3            0x33
#define KEY_4            0x34
#define KEY_5            0x35
#define KEY_6            0x36
#define KEY_7            0x37
#define KEY_8            0x38
#define KEY_TAB          9
#define KEY_DELETE       127
#define KEY_UP_ARROW     11
#define KEY_DOWN_ARROW   10
#define KEY_LEFT_ARROW   8
#define KEY_RIGHT_ARROW  21

typedef enum
  {
    TOPICS,
    ARTICLES,
    ARTICLE,
    DONE
  } State;

#endif /* TYPEDEFS_H */
