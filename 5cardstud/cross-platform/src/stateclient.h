#ifndef STAECLIENT_H
#define STAECLIENT_H
#include <stdbool.h>

void updateState(bool isTables);
bool getStateFromServer();
bool apiCall(const char *path);

#endif /* STAECLIENT_H */
