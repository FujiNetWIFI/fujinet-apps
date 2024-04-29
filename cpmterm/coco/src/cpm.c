/**
 * @brief   CPM
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose CPM Device Functions
 */

#include <cmoc.h>
#include <coco.h>
#include "dw.h"

#define OP_CPM 0xE4
#define CMD_READY 0x00
#define CMD_RESPONSE 0x01
#define CMD_BOOT 'B'
#define CMD_STATUS 'S'
#define CMD_READ 'R'
#define CMD_WRITE 'W'

/**
 * @brief wait for network device ready
 */
void cpm_ready(void)
{
    struct _readycmd
    {
        byte opcode;
        byte command;
    } rc;

    byte z=0, r=0;
    
    rc.opcode = OP_CPM;
    rc.command = CMD_READY;
    
    while (!z)
    {
        dwwrite((byte *)&rc,sizeof(rc));
        z = dwread((byte *)&r,sizeof(r));
    }
}

/**
 * @brief Get response data from last command
 * @param buf Target buffer 
 * @param len Length 
 */
byte cpm_get_response(byte *buf, int len)
{
    struct _getresponsecmd
    {
        byte opcode;
        byte command;
    } grc;

    byte z=0;
    
    grc.opcode = OP_CPM;
    grc.command = CMD_RESPONSE;

    cpm_ready();
    dwwrite((byte *)&grc, sizeof(grc));
    dwread((byte *)buf, len);
    
    return z;
}

/**
 * @brief Ask CP/M device to boot CP/M 
 */
void cpm_boot(void)
{
    struct _bootcmd
    {
        byte opcode;
        byte command;
    } bc;

    bc.opcode = OP_CPM;
    bc.command = CMD_BOOT;

    cpm_ready();
    dwwrite((byte *)&bc, sizeof(bc));
}

/**
 * @brief Return CP/M status (# of bytes waiting in RX queue)
 * @return # of bytes waiting in RX queue.
 */
unsigned short cpm_status(void)
{
    unsigned short bw=0;
    
    struct _statuscmd
    {
        byte opcode;
        byte command;
    } sc;

    sc.opcode = OP_CPM;
    sc.command = CMD_STATUS;

    cpm_ready();
    dwwrite((byte *)&sc, sizeof(sc));
    cpm_get_response((byte *)&bw, 2);
    
    return bw;
}

/**
 * @brief read from CP/M
 * @param buf Destination buffer (must be able to hold len bytes)
 * @param len # of bytes to read (0-65535)
 * @return error result.
 */
byte cpm_read(byte *buf, unsigned int len)
{
    struct _readcmd
    {
        byte opcode;
        byte command;
        unsigned int len;
    } rc;

    rc.opcode = OP_CPM;
    rc.command = CMD_READ;
    rc.len = len;

    cpm_ready();
    dwwrite((byte *)&rc, sizeof(rc));
    cpm_get_response(buf, len);

    return 1; // for now.
}

/**
 * @brief write to CP/M
 * @param buf Destination buffer (must be able to hold len bytes)
 * @param len # of bytes to read (0-65535)
 * @return error result.
 */
byte cpm_write(byte *buf, unsigned int len)
{
     struct _writecmd
    {
        byte opcode;
        byte command;
        unsigned int len;
        byte b;
    } wc;

    wc.opcode = OP_CPM;
    wc.command = CMD_WRITE;
    wc.len = len;
    wc.b = buf[0];
    
    cpm_ready();
    dwwrite((byte *)&wc, sizeof(wc));
    return 1;
}
