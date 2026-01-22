#ifndef NET_H
#define NET_H

#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"


/**
 * @brief Set login for network channel
 * @param devid device ID (0-255)
 * @param s login to set (0-255 chars)
 */
void net_login(const char *devicespec, const char *login);

/**
 * @brief Set password for network channel
 * @param devid device ID (0-255)
 * @param s password to set (0-255 chars)
 */
void net_password(const char *devicespec, const char *password);

#endif /* NET_H */
