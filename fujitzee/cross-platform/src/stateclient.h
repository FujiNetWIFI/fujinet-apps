#ifndef STAECLIENT_H
#define STAECLIENT_H

#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#else
#include <stdbool.h>
#include <stdint.h>
#endif

#define API_CALL_ERROR (0)
#define API_CALL_SUCCESS (1)
#define API_CALL_PENDING (2)

#define STATE_UPDATE_ERROR (0)
#define STATE_UPDATE_CHANGE (1)
#define STATE_UPDATE_NOCHANGE (2)

void updateState(bool isTables);
uint8_t getStateFromServer();
uint8_t apiCall(char *path, bool isAsync);
void sendMove(char* move);

#endif /* STAECLIENT_H */
