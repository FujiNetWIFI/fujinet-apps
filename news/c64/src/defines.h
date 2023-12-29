/**
 * @brief   FujiNet News Reader - Commodore 16 Version
 * @author  Thomas Cherryhomes
 * @license gpl v. 3
 * @brief   defines (constants/typedefs/etc)
 */

#ifndef DEFINES_H
#define DEFINES_H

#define BUF_SIZE  2048

#define CH_RETURN 0x0D
#define CH_DOWN   0x11
#define CH_UP     0x91
#define CH_RT     0x1D
#define CH_LF     0x9D

#define TOPICS_MAX 9
#define TOPICS_TOP " FUJINET NEWS READER   SELECT A TOPIC   "
#define TOPICS_BOT " CRSR   SELECT TOPIC RETURN SEE RESULTS "
#define TOPICS_Y   3

#define TITLES_MAX 5
#define TITLES_SIZE 66
#define TITLES_TOP "   ARTICLE RESULTS    ESC FOR TOPICS    "
#define TITLES_BOT " CRSR   SELECT TOPIC RETURN READ ARTICLE"
#define TITLES_Y  1

#define LFN       2
#define DEV       16    // The network device #
#define SAN       2
#define CMD       15

typedef enum _state
  {
    TOPICS,
    TITLES,
    ARTICLE,
  } State;

#endif /* DEFINES_H */
