#ifndef WEBCLIENT_H_
#define WEBCLIENT_H_

#include "process.h"

/* Callback functions that have to be implemented by the application program. */
void webclient_datahandler(char *data, uint16_t len);
void webclient_connected(void);
void webclient_timedout(void);
void webclient_closed(void);

unsigned char webclient_open(const char *url);
void webclient_close(void);
char *webclient_mimetype(void);

PROCESS_NAME(webclient_process);

#endif /* WEBCLIENT_H_ */
