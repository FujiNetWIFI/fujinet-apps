/**
 * @brief FujiNet Lobby for Adam
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FETCH_H
#define FETCH_H

#define FETCH_NUM_PER_PAGE 5

typedef struct _fetchPage
{
  char g[32];
  int t;
  char u[128];
  char c[128];
  char s[32];
  char r[4];
  int o;
  int m;
  int p;
  int a;
} FetchPage;

void fetch(void);

#endif /* FETCH_H */
