#ifndef STAECLIENT_H
#define STAECLIENT_H

#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#else
#include <stdbool.h>
#endif

void updateState(unsigned char isTables);
unsigned char getStateFromServer();
unsigned char apiCall(const char *path);

#endif /* STAECLIENT_H */
