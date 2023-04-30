/**
 * Fuji Device ($70) Commands
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>
#include "appkey.h"
#include "nio.h"
#include "fuji.h"

union
{
  struct
  {
    unsigned char host_slot;
    unsigned char mode;
    char file[36];
  } slot[8];
  unsigned char rawData[304];
} device_slots;


/**
 * Read host slots
 */
void host_read(char* host_slots)
{
  // Query for host slots
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xF4; // Get host slots
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=host_slots;
  OS.dcb.dtimlo=0x0f;
  OS.dcb.dbyt=256;
  OS.dcb.daux=0;
  siov();
}

/**
 * Write host slots
 */
void host_write(char* host_slots)
{
  // Query for host slots
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xF3; // Write host slots
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=host_slots;
  OS.dcb.dtimlo=0x0f;
  OS.dcb.dbyt=256;
  OS.dcb.daux=0;
  siov();
}

/**
 * Read Device Slots
 */
void disk_read(void)
{
  // Read Drive Tables
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xF2;
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=&device_slots.rawData;
  OS.dcb.dtimlo=0x0f;
  OS.dcb.dbyt=sizeof(device_slots.rawData);
  OS.dcb.daux=0;
  siov();
}

/**
 * Write device slots
 */
void disk_write(void)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xF1;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=&device_slots.rawData;
  OS.dcb.dtimlo=0x0f;
  OS.dcb.dbyt=sizeof(device_slots.rawData);
  OS.dcb.daux=0;
  siov();
}

/// @brief Set the host slot and read/write mode for the given device slot
/// @param device_slot The device slot to update
/// @param host_slot The host slot to use
/// @param mode - The appropriate FUJI_HOST_MODE_*
void disk_set_host_slot(unsigned char device_slot, unsigned char host_slot, unsigned char mode)
{
  disk_read();
  device_slots.slot[device_slot].host_slot = host_slot;
  device_slots.slot[device_slot].mode = mode;
  disk_write();
}


/**
 * Set filename for device slot
 */
void set_filename(char* filename, unsigned char slot)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xE2;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=filename;
  OS.dcb.dtimlo=0x0F;
  OS.dcb.dbyt=256;
  OS.dcb.daux1=slot;
  OS.dcb.daux2=0;
  siov();
}

/**
 * Mount a host slot
 */
void host_mount(unsigned char c)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xF9;
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=0;
  OS.dcb.dtimlo=0x01;
  OS.dcb.dbyt=0;
  OS.dcb.daux=c;
  siov();
}

/**
 * Unmount a host slot
 */
void host_unmount(unsigned char c)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xE6;
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=0;
  OS.dcb.dtimlo=0x01;
  OS.dcb.dbyt=0;
  OS.dcb.daux=c;
  siov();
}

/**
 * Mount device slot
 */
void disk_mount(unsigned char c, unsigned char o)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xF8;
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=0;
  OS.dcb.dtimlo=0x01;
  OS.dcb.dbyt=0;
  OS.dcb.daux1=c;
  OS.dcb.daux2=o;
  siov();
}

/**
 * Reset the fujinet, re-mounting the CONFIG boot disk. You must wait 5 seconds after calling
 */
void fujinet_reset()
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xFF;
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=0;
  OS.dcb.dtimlo=0x01;
  OS.dcb.dbyt=0;
  OS.dcb.daux=0;
  siov();
}


/*
/// @brief Invoke SIO
/// @param ddevic device id
/// @param dunit unit number
/// @param dcomnd  command
/// @param dstats pointer to buffer
/// @param dbuf device timeout in seconds
/// @param dtimlo device timeout in seconds
/// @param dbyt # of bytes to transfer
/// @param daux1 1st command auxiliary byte
/// @param daux2 2nd command auxiliary byte
/// @return status return
unsigned char sio(unsigned char ddevic, unsigned char dunit, unsigned char dcomnd, unsigned char dstats, void *dbuf, unsigned char dtimlo, unsigned int  dbyt, unsigned char daux1, unsigned char daux2)
{
  OS.dcb.ddevic=ddevic;
  OS.dcb.dunit=dunit;
  OS.dcb.dcomnd=dcomnd;
  OS.dcb.dstats=dstats;
  OS.dcb.dbuf=dbuf;
  OS.dcb.dtimlo=dtimlo;
  OS.dcb.dbyt=dbyt;
  OS.dcb.daux1=daux1;
  OS.dcb.daux1=daux2;
  siov();

  return OS.dcb.dstats;
}

*/