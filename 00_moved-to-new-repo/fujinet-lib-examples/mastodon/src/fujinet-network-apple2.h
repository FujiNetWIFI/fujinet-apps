
/**
 * @brief FujiNet Network Device Library Apple2 Base Functions
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_NETWORK_APPLE2_H
#define FUJINET_NETWORK_APPLE2_H

#include <stdint.h>

// These are for C files to be able to access ASM functions, and values
// that are internal and not exposed in the normal fujinet-network.h header

#define MAX_DATA_LEN (767)

// The id of the network device
extern uint8_t sp_network;

// the general payload buffer
extern uint8_t sp_payload[];

// the dispatch function used for doing SP calls for a particular card
extern uint8_t sp_dispatch_fn[2];

// count of bytes the status request returned
extern uint16_t sp_count;

void sp_clr_payload();
int8_t sp_status(uint8_t dest, uint8_t statcode);
int8_t sp_control(uint8_t dest, uint8_t ctrlcode);
int8_t sp_read(uint8_t dest, uint16_t len);
uint8_t sp_init();

uint8_t network_status_no_clr(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);
uint8_t network_unit(char *devicespec);

#endif
