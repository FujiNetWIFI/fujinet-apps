/**
 * @brief   Network Routines
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <cmoc.h>
#include <coco.h>

#define OP_FUJI 0xE2
#define OP_NET 0xE3

#define BUS_SUCCESS 1

#define FUJICMD_READY 0x00

#define NETCMD_SEND_ERROR 0x02
#define NETCMD_CD 0x2C
#define NETCMD_SEND_RESPONSE 0x01
#define NETCMD_PWD 0x30

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

uint16_t fn_bytes_read=0;
uint16_t fn_network_bw=0;
uint8_t fn_network_conn=0;
uint8_t fn_device_error=0;
uint8_t fn_network_error=0;

/**
 * @brief read l bytes from drivewire into s
 * @param s Destination buffer
 * @param l # of bytes to read
 * @return error code
 */
byte dwread(byte *s, int l)
{
    asm
    {
        pshs x,y
            ldx :s
            ldy :l
            jsr [0xD93F]
            puls y,x
            tfr cc,b
            lsrb
            lsrb
            andb #$01
            }
}

/**
 * @brief read l bytes from s into drivewire
 * @param s Destination buffer
 * @param l # of bytes to write
 * @return error code
 */
byte dwwrite(byte *s, int l)
{
    asm
    {
        pshs x,y
            ldx :s
            ldy :l
            jsr [0xD941]
            tfr cc,d
            puls y,x
            }
}

/**
 * @brief wait for drivewire bus to be ready
 */
void bus_ready(void)
{
    struct _readycmd
    {
        uint8_t opcode;
        uint8_t command;
    } rc;

    uint8_t z=0, r=0;
    
    rc.opcode = OP_FUJI;
    rc.command = FUJICMD_READY;
    
    while (!z)
    {
        dwwrite((uint8_t *)&rc,sizeof(rc));
        z = dwread((uint8_t *)&r,sizeof(r));
    }  
}

/**
 * @brief return network unit number given devicespec string
 * @param devicespec A string containing device spec.
 * @return Unit number, 0=invalid, >0=valid.
 */
uint8_t network_unit(char *devicespec)
{
    if (devicespec[1] == ':')
        return 1;

    if (devicespec[2] == ':') {
        return devicespec[1] - '0';
    }

    return 1;
}

/**
 * @brief   Get Bus Response for OP_FUJI or OP_NET
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @param   devicespec devicespec to read response.
 * @param   buf Target buffer (needs to be at least len)
 * @param   len Buffer length
 */

uint8_t net_get_response(char *devicespec, uint8_t *buf, int len)
{
    struct _getresponsecmd
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t command;
        uint16_t len;
    } grc;

    uint8_t z=0;
    
    grc.opcode = OP_NET;
    grc.unit = network_unit(devicespec);
    grc.command = NETCMD_SEND_RESPONSE;
    grc.len = len;
    
    bus_ready();
    dwwrite((uint8_t *)&grc, sizeof(grc));
    dwread((uint8_t *)buf, len);
    
    return 1;
}

/**
 * @brief   Return error code of previous sub device operation
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose opcode is OP_FUJI or OP_NET
 */

uint8_t net_error(char *devicespec)
{
    struct _senderrorcmd
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t aux1;
        uint8_t aux2;
    } sec;

    uint8_t err = 0;
    
    sec.opcode = OP_NET;
    sec.cmd = NETCMD_SEND_ERROR;
    sec.unit = network_unit(devicespec);
    sec.aux1 = sec.aux2 = 0;

    bus_ready();
    dwwrite((uint8_t *)&sec, sizeof(sec));
    dwread((uint8_t *)&err, sizeof(err));

    return err;
}

/**
 * @brief Set network prefix (aka CD)
 * @param devicespec with new prefix.
 * @return result error code
 */
uint8_t net_cd(char *devicespec)
{
    struct _cdcmd
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t aux1;
        uint8_t aux2;
        char prefix[256];
    } c;

    c.opcode = OP_NET;
    c.unit = network_unit(devicespec);
    c.cmd = NETCMD_CD;
    c.aux1 = c.aux2 = 0;
    strcpy(c.prefix,devicespec);

    bus_ready();
    dwwrite((uint8_t *)&c, sizeof(struct _cdcmd));

    return 1;
}

/**
 * @brief Return network prefix for desired devicespec.
 * @param devicespec The N: devicespec
 * @param s the target string array buffer
 * @return error code.
 */
uint8_t net_pwd(char *devicespec, char *s)
{
    struct _pwdcmd
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint16_t len;
    } p;

    p.opcode = OP_NET;
    p.unit = network_unit(devicespec);
    p.cmd = NETCMD_PWD;
    p.len = 256;

    bus_ready();
    dwwrite((uint8_t *)&p, sizeof(struct _pwdcmd));

    net_get_response(devicespec,(uint8_t *)s,256);

    return 1;
}

uint8_t network_open(char* devicespec, uint8_t mode, uint8_t trans)
{
    struct _o
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t mode;
        uint8_t trans;
        char devicespec[256];
    } o;

    o.opcode = OP_NET;
    o.unit = network_unit(devicespec);
    o.cmd = 'O';
    o.mode = mode;
    o.trans = trans;
    strcpy(o.devicespec,devicespec);

    bus_ready();
    dwwrite((uint8_t *)&o, sizeof(o));
    
    return net_error(devicespec) == BUS_SUCCESS;
}

uint8_t network_close(char* devicespec)
{
    struct _nc
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
    } nc;

    nc.opcode = OP_NET;
    nc.unit = network_unit(devicespec);
    nc.cmd = 'C';

    bus_ready();

    dwwrite((uint8_t *)&nc, sizeof(nc));
    
    return net_error(devicespec) == BUS_SUCCESS;
}

uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err)
{
    struct _s
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t aux1;
        uint8_t aux2;
    } s;

    struct _sr
    {
        uint16_t bw;
        uint8_t c;
        uint8_t err;
    } sr;

    s.opcode = OP_NET;
    s.unit = network_unit(devicespec);
    s.cmd = 'S';
    s.aux1 = s.aux2 = 0;

    bus_ready();
    dwwrite((uint8_t *)&s, sizeof(s));
    net_get_response(devicespec, (uint8_t *)&sr, sizeof(sr));

    *bw = sr.bw;
    *c = sr.c;
    *err = sr.err;
    
    return net_error(devicespec) == BUS_SUCCESS;
}

int16_t network_read(char *devicespec, uint8_t *buf, uint16_t len)
{
    struct _rcmd
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint16_t len;
    } r;

    r.opcode = OP_NET;
    r.unit = network_unit(devicespec);
    r.cmd = 'R';
    r.len = len;

    bus_ready();
    dwwrite((uint8_t *)&r, sizeof(struct _rcmd));
    net_get_response(devicespec,buf,len);

    return len;    
}
