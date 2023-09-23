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
#include "fn_io.h"

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
  fn_io_get_host_slots((HostSlot *) host_slots);
}

/**
 * Write host slots
 */
void host_write(char* host_slots)
{
  fn_io_put_host_slots((HostSlot *) host_slots);
}

/**
 * Read Device Slots
 */
void disk_read(void)
{
  fn_io_get_device_slots((DeviceSlot *) &device_slots.rawData);
}

/**
 * Write device slots
 */
void disk_write(void)
{
  fn_io_put_device_slots((DeviceSlot *) &device_slots.rawData);
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
void set_filename(unsigned char mode, unsigned char host_slot, unsigned char device_slot, char* filename)
{
  fn_io_set_device_filename(mode, host_slot, device_slot, filename);
}

/**
 * Mount a host slot
 */
void host_mount(unsigned char c)
{
  fn_io_mount_host_slot(c);
}

/**
 * Unmount a host slot
 */
void host_unmount(unsigned char c)
{
  fn_io_unmount_host_slot(c);
}

/**
 * Mount device slot
 */
void disk_mount(unsigned char c, unsigned char o)
{
  fn_io_mount_disk_image(c, o);
}

/**
 * Reset the fujinet, re-mounting the CONFIG boot disk. You must wait 5 seconds after calling
 */
void fujinet_reset()
{
  fn_io_reset();
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