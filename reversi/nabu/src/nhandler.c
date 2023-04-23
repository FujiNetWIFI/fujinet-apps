

#include "nhandler.h"
#include "network.h"
#include <string.h>


char eol = 13;
int clock_unit      = FN_ERR_NO_DEVICE;
int device_state[4] = {FN_ERR_ACCESS_DENIED, FN_ERR_ACCESS_DENIED, FN_ERR_ACCESS_DENIED, FN_ERR_ACCESS_DENIED};
int last_error[4]   = {FN_ERR_NO_DEVICE, FN_ERR_NO_DEVICE, FN_ERR_NO_DEVICE, FN_ERR_NO_DEVICE};

int translate_adam_error_to_nhandler_error(unsigned char error_in)
{
    return (int) error_in;
}

void nopen(int basic_unit, int rw_mode, int trans, char *url)
{
    unsigned char adam_error;

    if (basic_unit == 0)
        adam_error = network_open(url, (unsigned char)rw_mode, (unsigned char)trans);
        if (adam_error != 0)
            device_state[0] = NET_DEV; 
    else
        adam_error = FN_ERR_NO_DEVICE;

    last_error[basic_unit] = translate_adam_error_to_nhandler_error(adam_error);
}

void nclose(int basic_unit)
{
    unsigned char adam_error;

    adam_error = 0;
    last_error[basic_unit] = translate_adam_error_to_nhandler_error(adam_error);
}

void nstatus(int basic_unit, int *bytes_waiting, int *connected, int *last_error_out)
{
    NetStatus status;
    unsigned char adam_error;

    adam_error = network_status(&status);
    
    if (adam_error == 0)
    {
        *bytes_waiting = status.rxBytesWaiting;
        *connected     = status.connected ? 1 : 0;
        *last_error_out= last_error[basic_unit]; 
    }
}

void nread(int basic_unit, char *buffer, int length)
{
    unsigned char adam_error;
    unsigned short len = (unsigned short) length;


    adam_error = network_read(buffer, len);

    last_error[basic_unit] = translate_adam_error_to_nhandler_error(adam_error);
}

void nwrite(int basic_unit, char *buffer, int length)
{
    unsigned char adam_error;

    adam_error = network_write((unsigned char)buffer, (unsigned short)length);
    last_error[basic_unit] = translate_adam_error_to_nhandler_error(adam_error);
}

void nctrl(int basic_unit, int command, char *buffer)
{
    last_error[basic_unit] =  FN_ERR_CMD_NOT_IMPLEMENTED;
}

void naccept(int basic_unit)
{

}

void nseteol(int new_eol)
{
    eol = new_eol;
}

void nprint(int basic_unit, char *buffer)
{
    unsigned char adam_error;

    nwrite(basic_unit, (unsigned char *) buffer, strlen(buffer));
    nwrite(basic_unit, (unsigned char *) &eol, 1);

}

void ninput(int basic_unit, char *buffer)
{
    int index = 0;
    unsigned char adam_error;

    while(true)
    {
        adam_error = network_read(buffer[index], 1);
        if (adam_error != 0)
            break;
        if (buffer[index] == eol)
            break;
        index++;
        if (index > 255)
            break;
    }
    buffer[index] = '\0';
}

void njson(int basic_unit, int onoff)
{
    last_error[basic_unit] = FN_ERR_CMD_NOT_IMPLEMENTED;
}

void nquery(int basic_unit, char *query, char *value)
{
    last_error[basic_unit] = FN_ERR_CMD_NOT_IMPLEMENTED;
}

void ntime(int *year, int *month, int *day, int *hour, int *minute, int *second)
{
    unsigned char adam_error;

    adam_error = FN_ERR_NO_DEVICE;
    *year = *month = *day = *hour = *minute = *second = 0;

}
