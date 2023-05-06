void nopen(int basic_unit, int rw_mode, int trans, char *url);
void nclose(int basic_unit);
void nstatus(int basic_unit, int *bytes_waiting, int *connected, int *last_error);
void nread(int basic_unit, char *buffer, int length);
void nwrite(int basic_unit, char *buffer, int length); 
void nctrl(int basic_unit, int command, char *buffer); 
void naccept(int basic_unit);
void nseteol(int new_eol);
void nprint(int basic_unit, char *buffer);
void ninput(int basic_unit, char *buffer);
void njson(int basic_unit, int onoff);
void nquery(int basic_unit, char *query, char *value);
void ntime(int *year, int *month, int *day, int *hour, int *minute, int *second);

#define FN_READ                 4
#define FN_WRITE                8

#define FN_OK                   0
#define FN_ERR                  0xFE
#define FN_ERR_NO_DEVICE        0xFD

#define FN_ERR_WRITE_ONLY       131	    // 0x83 Protocol is in Write-only mode
#define FN_ERR_INVALID_COMMAND  132	    // 0x84 Protocol was sent an invalid command
#define FN_ERR_MISSING_PROTO    133	    // 0x85 No protocol attached.
#define FN_ERR_READ_ONLY        135	    // 0x86 Protocol is in read-only mode
#define FN_ERR_TIMED_OUT        138	    // 0x8A Timed out
#define FN_ERR_CRITICAL         144	    // 0x90 A critical error occurred. SIO reports this, get the actual Error code from byte 4 of STATUS.
#define FN_ERR_CMD_NOT_IMPLEMENTED 146	// 0x92 Command not implemented
#define FN_ERR_FILE_EXISTS      151	    // 0x97 File Exists
#define FN_ERR_NO_SPACE         162	    // 0xA2 No space on device
#define FN_ERR_INVALID_URL      165	    // 0xA5 Invalid Devicespec
#define FN_ERR_ACCESS_DENIED    167	    // 0xA7 Access Denied
#define FN_ERR_FILE_NOT_FOUND   170	    // 0xAA File not found (emitted by filesystem adaptors)
#define FN_ERR_REFUSED          200	    // 0xC8 Connection Refused (equivalent to errno ECONNREFUSED)
#define FN_ERR_UNREACHABLE      201	    // 0xC9 Network Unreachable (equivalent to errno ENETUNREACH)
#define FN_ERR_CONNECTION_TIMEOUT   202	// 0xCA Connection Timeout (equivalent to errno ETIMEDOUT)
#define FN_ERR_NETWORK_DOWN     203	    // 0xCB Network is down (equivalent to errno ENETDOWN)
#define FN_ERR_CONNECTION_RESET 204	    // 0xCC Connection was reset during read/write (equivalent to errno ECONNRESET)
#define FN_ERR_CIP              205	    // 0xCD Connection in progress (EAGAIN)
#define FN_ERR_ADDRESS_IN_USE   206	    // 0xCE Address in use (EADDRINUSE)
#define FN_ERR_NOT_CONNECTED    207	    // 0xCF Not Connected
#define FN_ERR_SERVER_NOT_RUNNING 208	    // 0xD0 Server not Running
#define FN_ERR_NO_CONNECTION_WAITING 209	// 0xD1 No connection waiting
#define FN_ERR_SERVICE_NOT_AVAILABLE 210	// 0xD2 Service Not Available
#define FN_ERR_CONNECTION_ABORTED 211	    // 0xD3 Connection Aborted
#define FN_ERR_BAD_CREDENTIALS  212	    // 0xD4 Invalid Username or Password (debating whether to overload as access denied.)
#define FN_ERR_MEMORY_ERROR     255	    // 0xFF Could not allocate either receive or transmit buffers.
