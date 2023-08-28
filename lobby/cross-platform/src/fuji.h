/**
 * @brief   Defines high Level FujiNet function that must be implemented by each platform
 * @author  Eric Carr
 * @license gpl v. 3
 */

#ifndef FUJI_H
#define FUJI_H

#include "io.h"

/* The following functions must be implemented by each platform */

/// @brief Retrives and parses an entire json response as a set of \n terminated lines
/// @param url Destination buffer. It must be large enough to hold the parsed response
/// @return 0 on success, non zero for error code
unsigned char getJsonResponse(char *url, unsigned char* buf, int maxLen);


/// @brief Platform specific initialization
void initialize();


#endif /* FUJI_H */