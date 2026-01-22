#include <cmoc.h>
#include <coco.h>
#include "net.h"
#include "fujinet-fuji.h"
#include "dw.h"

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
#define CMD_WRITE 'W'
#define CMD_SET_CHANNEL_MODE 0xFC
#define CMD_SET_LOGIN 0xFD
#define CMD_SET_PASSWORD 0xFE

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
 * @brief Set login for network channel
 * @param devid device ID (0-255)
 * @param s login to set (0-255 chars)
 */
void net_login(const char *devicespec, const char *login)
{
    byte devid = network_unit(devicespec);

    struct _logincmd
    {
        byte opcode;
        byte id;
        byte command;
        byte aux1;
        byte aux2;
        char login[256];
    } lc;

    lc.opcode = OP_NET;
    lc.id = devid;
    lc.command = CMD_SET_LOGIN;
    lc.aux1 = lc.aux2 = 0;
    memset(lc.login,0,sizeof(lc.login));
    strcpy(lc.login,login);

    net_ready(devid);
    dwwrite((byte *)&lc, sizeof(lc));
}

/**
 * @brief Set password for network channel
 * @param devid device ID (0-255)
 * @param s password to set (0-255 chars)
 */
void net_password(const char *devicespec, const char *password)
{
    byte devid = network_unit(devicespec);
    
    struct _passwordcmd
    {
        byte opcode;
        byte id;
        byte command;
        byte aux1;
        byte aux2;
        char password[256];
    } pc;

    pc.opcode = OP_NET;
    pc.id = devid;
    pc.command = CMD_SET_PASSWORD;
    pc.aux1 = pc.aux2 = 0;
    memset(pc.password,0,sizeof(pc.password));
    strcpy(pc.password,password);

    net_ready(devid);
    dwwrite((byte *)&pc, sizeof(pc));
}
