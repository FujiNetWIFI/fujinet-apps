/**
 * @brief   Net
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose public Net device functions
 */

#ifndef NET_H
#define NET_H

#include <cmoc.h>
#include <coco.h>

/**
 * @brief Set network prefix (aka CD)
 * @param devicespec with new prefix.
 * @return result error code
 */
uint8_t net_cd(char *devicespec);

/**
 * @brief Return network prefix for desired devicespec.
 * @param devicespec The N: devicespec
 * @param s the target string array buffer
 * @return error code.
 */
uint8_t net_pwd(char *devicespec, char *s);

uint8_t network_open(char* devicespec, uint8_t mode, uint8_t trans);
uint8_t network_close(char* devicespec);
uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);
int16_t network_read(char *devicespec, uint8_t *buf, uint16_t len);

#endif /* NET_H */
