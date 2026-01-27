#ifndef NETWORK_H
#define NETWORK_H

#include "fujinet-fuji.h"

void setup_url(bool articles);
char *fetch_data(bool articles);

#endif // NETWORK_H