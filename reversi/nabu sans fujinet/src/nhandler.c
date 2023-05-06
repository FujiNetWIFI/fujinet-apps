

#include "nhandler.h"
#include "network.h"
#include <stdio.h>
#include <string.h>


char eol = 13;
int clock_unit      = FN_ERR_NO_DEVICE;
int device_state[4] = {FN_ERR_ACCESS_DENIED, FN_ERR_ACCESS_DENIED, FN_ERR_ACCESS_DENIED, FN_ERR_ACCESS_DENIED};
int last_error[4]   = {FN_ERR_NO_DEVICE, FN_ERR_NO_DEVICE, FN_ERR_NO_DEVICE, FN_ERR_NO_DEVICE};

int translate_nabu_error_to_nhandler_error(unsigned char error_in)
{
    return (int) error_in;
}

void ninit(void)
{
    network_init();
}

void nopen(int basic_unit, int rw_mode, int trans, char *url)
{
    if (basic_unit != 0) 
        return;

    network_open(url, rw_mode, trans);
}

void nclose(int basic_unit)
{
    if (basic_unit == 0)
        network_close();
}

void nstatus(int basic_unit, int *bytes_waiting, int *connected, int *last_error_out)
{
    NetStatus s;

    if (basic_unit != 0)
        return;

    network_status(&s);

    *bytes_waiting = s.rxBytesWaiting;
    *connected = s.connected;
    *last_error_out = last_error[basic_unit];

}

void nread(int basic_unit, char *buffer, int length)
{
    if (basic_unit != 0)
        return;

    network_read(buffer, (unsigned short) length);

}

void nwrite(int basic_unit, char *buffer, int length)
{
    if (basic_unit != 0)
        return;

    network_write(buffer, (unsigned short)length);
}

void nctrl(int basic_unit, int command, char *buffer)
{
    if (basic_unit != 0)
        return;

    command = command;
    buffer = buffer;
        
    return;
}

void naccept(int basic_unit)
{
    if (basic_unit != 0)
        return;

    return;
}

void nseteol(int new_eol)
{
    eol = new_eol;
}

void nprint(int basic_unit, char *buffer)
{
    char temp[80];

    if (basic_unit != 0)
        return;

    sprintf(temp, "%s%c", buffer, eol);
    nwrite(basic_unit, temp, strlen(temp));
}

void ninput(int basic_unit, char *buffer)
{
    int index = 0;
    int err;

    if (basic_unit != 0)
        return;

    buffer[0] = '\0';

    while(true)
    {
        err = network_read(&buffer[index], 1);
        if (err == NET_ERR_OK)
        {
            if (buffer[index] == eol)
            {
                buffer[index] = '\0';
                break;
            } else
            {
                index++;
                buffer[index] = '\0';
            }
        }
    }

}

void njson(int basic_unit, int onoff)
{
    if (basic_unit != 0)
        return;
    onoff = onoff;
}

void nquery(int basic_unit, char *query, char *value)
{
    if (basic_unit != 0)
        return;
    query = query;
    value = value;
}

void ntime(int *year, int *month, int *day, int *hour, int *minute, int *second)
{
    *year = *month = *day = *hour = *minute = *second = 0;
}
