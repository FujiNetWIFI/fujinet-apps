#include <cmoc.h>
#include <coco.h>
#include "dw.h"
#include "net.h"

#define OP_NET 0xE3

#define CMD_READY 0x00
#define CMD_RESPONSE 0x01
#define CMD_ERROR 0x02
#define CMD_OPEN 'O'
#define CMD_CLOSE 'C'
#define CMD_PARSE_JSON 'P'
#define CMD_QUERY_JSON 'Q'
#define CMD_READ 'R'
#define CMD_STATUS 'S'
#define CMD_SET_CHANNEL_MODE 0xFC

/**
 * @brief wait for network device ready
 * @param devid Device ID (0=255)
 */
void net_ready(byte devid)
{
    struct _readycmd
    {
        byte opcode;
        byte id;
        byte command;
        byte aux1;
        byte aux2;
    } rc;

    byte z=0, r=0;
    
    rc.opcode = OP_NET;
    rc.id = devid;
    rc.command = CMD_READY;
    rc.aux1 = rc.aux2 = 0;
    
    while (!z)
    {
        dwwrite((byte *)&rc,sizeof(rc));
        z = dwread((byte *)&r,sizeof(r));
    }
}
/**
 * @brief get last error code
 * @param devid Device ID (0-255)
 * @return error code byte
 */
byte net_error(byte devid)
{
    struct _errcmd
    {
        byte opcode;
        byte id;
        byte command;
        byte aux1;
        byte aux2;
    } ec;

    byte z=0;
    byte err=0;
    
    ec.opcode = OP_NET;
    ec.id = devid;
    ec.command = CMD_ERROR;
    ec.aux1 = ec.aux2 = 0;

    net_ready(devid);
    
    dwwrite((byte *)&ec, sizeof(ec));
    z = dwread(&err,1);
   
    return z ? z : err;
}

/**
 * @brief Get response data from last command
 * @param devid The device ID (0-255) 
 * @param buf Target buffer 
 * @param len Length 
 */
byte net_get_response(byte devid, byte *buf, int len)
{
    struct _getresponsecmd
    {
        byte opcode;
        byte id;
        byte command;
        int len;
    } grc;

    byte z=0;
    
    grc.opcode = OP_NET;
    grc.id = devid;
    grc.command = CMD_RESPONSE;
    grc.len = len;

    net_ready(devid);
    dwwrite((byte *)&grc, sizeof(grc));
    dwread((byte *)buf, len);
    
    return z;
}

/**
 * @brief Open network connection
 * @param devid Device ID (0-255)
 * @param mode Open Mode (4=READ, 6=DIR, 8=WRITE, 9=APPEND, 12=READ/WRITE, 13=POST) 
 * @param trans Translation mode (0=none) 
 * @param url 256 byte max, N: URL
 * @return result code
 */
byte net_open(byte devid, byte mode, byte trans, const char *url)
{
    struct _opencmd
    {
        byte opcode;
        byte id;
        byte command;
        byte mode;
        byte trans;
        char url[256];
    } oc;

    oc.opcode = OP_NET;
    oc.id = devid;
    oc.command = CMD_OPEN;
    oc.mode = mode;
    oc.trans = trans;

    memset(oc.url,0,256);
    strcpy(oc.url,url);

    // wait for ready
    net_ready(devid);
    
    // Send open command
    dwwrite((byte *)&oc, sizeof(oc));

    // check status of open
    net_ready(devid);
    
    // Return result of open
    return net_error(devid);
}

/**
 * @brief Close network connection
 * @param devid Device ID (0-255)
 */
void net_close(byte devid)
{
    struct _closecmd
    {
        byte opcode;
        byte id;
        byte command;
        byte aux1;
        byte aux2;
    } cc;

    cc.opcode = OP_NET;
    cc.id = devid;
    cc.command = CMD_CLOSE;
    cc.aux1 = cc.aux2 = 0;

    // Wait for ready
    net_ready(devid);

    // Send command
    dwwrite((byte *)&cc, sizeof(cc));
}

/**
 * @brief Return Network status
 * @param devid Device ID (0-255)
 * @param ns Pointer to NetworkStatus struct
 * @return error result.
 */
byte net_status(byte devid, NetworkStatus *ns)
{
    byte z = 0;
    byte err = 0;
    
    struct _statuscmd
    {
        byte opcode;
        byte id;
        byte command;
        byte aux1;
        byte aux2;
    } sc;

    sc.opcode = OP_NET;
    sc.id = devid;
    sc.command = CMD_STATUS;
    sc.aux1 = sc.aux2 = 0;

    net_ready(devid);
    dwwrite((byte *)&sc, sizeof(sc));
    net_get_response(devid,(byte *)ns, sizeof(NetworkStatus));
    
    return net_error(devid);
}

/**
 * @brief read from network channel
 * @param devid Device ID (0-255)
 * @param buf Destination buffer (must be able to hold len bytes)
 * @param len # of bytes to read (0-65535)
 * @return error result.
 */
byte net_read(byte devid, byte *buf, unsigned int len)
{
    byte z = 0;

    struct _readcmd
    {
        byte opcode;
        byte id;
        byte command;
        unsigned int len;
    } rc;

    rc.opcode = OP_NET;
    rc.id = devid;
    rc.command = CMD_READ;
    rc.len = len;

    net_ready(devid);
    dwwrite((byte *)&rc, sizeof(rc));
    net_get_response(devid, buf, len);

    return net_error(devid);
}

/**
 * @brief Set network channel mode
 * @param devid Device ID (0-255)
 * @param channelMode channel mode, 0 = normal, 1 = JSON
 */
void net_set_channel_mode(byte devid, byte channelMode)
{
    struct _setchannelmodecmd
    {
        byte opcode;
        byte id;
        byte command;
        byte mode;
        byte aux2;
    } scm;

    scm.opcode = OP_NET;
    scm.id = devid;
    scm.command = CMD_SET_CHANNEL_MODE;
    scm.mode = channelMode;
    scm.aux2 = 0;

    net_ready(devid);    
    dwwrite((byte *)&scm, sizeof(scm));
}

/**
 * @brief Parse JSON waiting in channel
 * @param devid Device ID (0-255)
 * @return Result code (1=success, 213 = could not parse JSON)
 */
byte net_parse_json(byte devid)
{
    struct _parsejsoncmd
    {
        byte opcode;
        byte id;
        byte command;
        byte aux1;
        byte aux2;
    } pj;

    pj.opcode = OP_NET;
    pj.id = devid;
    pj.command = CMD_PARSE_JSON;
    pj.aux1 = pj.aux2 = 0;

    net_ready(devid);
    
    dwwrite((byte *)&pj, sizeof(pj));
    
    return net_error(devid);
}

/**
 * @brief query JSON
 * @param devid Device ID (0-255) 
 * @param qs pointer to query string 
 */
void net_set_json_query(byte devid, const char *qs)
{
    struct _queryjsoncmd
    {
        byte opcode;
        byte id;
        byte command;
        byte aux1;
        byte aux2;
        char qs[256];
    } qj;

    qj.opcode = OP_NET;
    qj.id = devid;
    qj.command = CMD_QUERY_JSON;
    qj.aux1 = qj.aux2 = 0;
    memset(qj.qs,0,256);
    strcpy(qj.qs,qs);
    
    net_ready(devid);
    dwwrite((byte *)&qj, sizeof(qj));
}
