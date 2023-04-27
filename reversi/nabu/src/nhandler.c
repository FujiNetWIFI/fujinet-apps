

#include "nhandler.h"
#include "network.h"
#include <string.h>


char eol = 13;
int clock_unit      = FN_ERR_NO_DEVICE;
int device_state[4] = {FN_ERR_ACCESS_DENIED, FN_ERR_ACCESS_DENIED, FN_ERR_ACCESS_DENIED, FN_ERR_ACCESS_DENIED};
int last_error[4]   = {FN_ERR_NO_DEVICE, FN_ERR_NO_DEVICE, FN_ERR_NO_DEVICE, FN_ERR_NO_DEVICE};

int translate_nabu_error_to_nhandler_error(unsigned char error_in)
{
    return (int) error_in;
}

void nopen(int basic_unit, int rw_mode, int trans, char *url)
{
}

void nclose(int basic_unit)
{
}

void nstatus(int basic_unit, int *bytes_waiting, int *connected, int *last_error_out)
{
}

void nread(int basic_unit, char *buffer, int length)
{
}

void nwrite(int basic_unit, char *buffer, int length)
{
}

void nctrl(int basic_unit, int command, char *buffer)
{
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
}

void ninput(int basic_unit, char *buffer)
{
}

void njson(int basic_unit, int onoff)
{
}

void nquery(int basic_unit, char *query, char *value)
{
}

void ntime(int *year, int *month, int *day, int *hour, int *minute, int *second)
{
}
