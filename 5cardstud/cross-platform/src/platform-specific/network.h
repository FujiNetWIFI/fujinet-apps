/*
  Network functionality
*/
#ifndef NETWORK_H
#define NETWORK_H

uint16_t getJsonResponse(char *url, char *buffer, uint16_t max_length);
uint8_t getResponse(char *url, char *buffer, uint16_t max_length);

#endif /* NETWORK_H */

