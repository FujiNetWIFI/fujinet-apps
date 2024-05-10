#ifndef STAECLIENT_H
#define STAECLIENT_H
#include <stdbool.h>

void updateState();
bool getStateFromServer();
bool apiCall(const char *path);

#endif /* STAECLIENT_H */
