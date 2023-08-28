#ifdef __ATARI__


/**
 * #FujiNet SIO calls
 */

#include <atari.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "sio.h"
#include "../io.h"


// variable to hold various responses that we just need to return a char*.
char response[512];

void set_sio_defaults(void)
{
  OS.dcb.ddevic = 0x70;
  OS.dcb.dunit = 1;
  OS.dcb.dtimlo = 0x0F; // 15 second timeout
}

bool io_error(void)
{
  return (OS.dcb.dstats > 128);
}

unsigned char io_status(void)
{
}

void io_init(void)
{
  OS.noclik = 0xFF;
  OS.shflok = 0;
  OS.color0 = 0x9f;
  OS.color1 = 0x0f;
  OS.color2 = 0x90;
  OS.color4 = 0x90;
  OS.coldst = 1;
  OS.sdmctl = 0; // Turn off screen
}

void io_get_device_slots(DeviceSlot *d)
{
  set_sio_defaults();
  OS.dcb.dcomnd = 0xF2;
  OS.dcb.dstats = 0x40;
  OS.dcb.dbuf = d;
  OS.dcb.dbyt = sizeof(deviceSlots);
  OS.dcb.daux = 0;
  siov();
}

void io_get_host_slots(HostSlot *h)
{
  // Query for host slots
  set_sio_defaults();
  OS.dcb.dcomnd = 0xF4; // Get host slots
  OS.dcb.dstats = 0x40;
  OS.dcb.dbuf = h;
  OS.dcb.dbyt = sizeof(hostSlots);
  OS.dcb.daux = 0;
  siov();
}


void io_put_host_slots(HostSlot *h)
{
  set_sio_defaults();
  OS.dcb.dcomnd = 0xF3;
  OS.dcb.dstats = 0x80;
  OS.dcb.dbuf = h;
  OS.dcb.dbyt = 256;
  OS.dcb.daux = 0;
  siov();
}

void io_put_device_slots(DeviceSlot *d)
{
  set_sio_defaults();
  OS.dcb.dcomnd = 0xF1;
  OS.dcb.dstats = 0x80;
  OS.dcb.dbuf = d;
  OS.dcb.dbyt = sizeof(deviceSlots);
  OS.dcb.daux = 0;
  siov();
}

void io_mount_host_slot(unsigned char hs)
{
  if (hostSlots[hs][0] != 0x00)
  {
    set_sio_defaults();
    OS.dcb.dcomnd = 0xF9;
    OS.dcb.dstats = 0x00;
    OS.dcb.dbuf = NULL;
    OS.dcb.dbyt = 0;
    OS.dcb.daux = hs;
    siov();
  }
}


void io_set_device_filename(unsigned char ds, char *e)
{
  OS.dcb.ddevic = 0x70;
  OS.dcb.dunit = 1;
  OS.dcb.dcomnd = 0xE2;
  OS.dcb.dstats = 0x80;
  OS.dcb.dbuf = e;
  OS.dcb.dtimlo = 0x0F;
  OS.dcb.dbyt = 256;
  OS.dcb.daux1 = ds;
  OS.dcb.daux2 = 0;
  siov();
}

char *io_get_device_filename(unsigned char slot)
{
  OS.dcb.ddevic = 0x70;
  OS.dcb.dunit = 1;
  OS.dcb.dcomnd = 0xDA;
  OS.dcb.dstats = 0x40;
  OS.dcb.dbuf = &response;
  OS.dcb.dtimlo = 0x0F;
  OS.dcb.dbyt = 256;
  OS.dcb.daux1 = slot;
  OS.dcb.daux2 = 0;
  siov();

  return &response;
}

void io_set_boot_config(unsigned char toggle)
{
  OS.dcb.dcomnd = 0xD9;
  OS.dcb.dstats = 0x00;
  OS.dcb.dbuf = NULL;
  OS.dcb.dtimlo = 0x0f;
  OS.dcb.dbyt = 0;
  OS.dcb.daux1 = toggle;
  siov();
}

void io_mount_disk_image(unsigned char ds, unsigned char mode)
{
  set_sio_defaults();
  OS.dcb.dcomnd = 0xF8;
  OS.dcb.dstats = 0x00;
  OS.dcb.dbuf = NULL;
  OS.dcb.dbyt = 0;
  OS.dcb.dtimlo = 0x0F; // Due to ATX support.
  OS.dcb.daux1 = ds;
  OS.dcb.daux2 = mode;
  siov();
}

void io_umount_disk_image(unsigned char ds)
{
  set_sio_defaults();
  OS.dcb.dcomnd = 0xE9;
  OS.dcb.dstats = 0x00;
  OS.dcb.dbuf = NULL;
  OS.dcb.dbyt = 0;
  OS.dcb.daux = ds;
  siov();
}

void io_boot(void)
{
  // Cold boot the computer after a second
  rtclr();
  while (OS.rtclok[2] < 60);
  cold_start();
}


bool io_get_device_enabled_status(unsigned char d)
{
  // adam calls $D1, which doesn't exist in atari.
  return false;
}

void io_enable_device(unsigned char d)
{
}

void io_disable_device(unsigned char d)
{
}

unsigned char io_device_slot_to_device(unsigned char ds)
{
}

/**
 * Get filename for device slot
 */
void io_get_filename_for_device_slot(unsigned char slot, const char *filename)
{
  OS.dcb.ddevic = 0x70;
  OS.dcb.dunit = 1;
  OS.dcb.dcomnd = 0xDA;
  OS.dcb.dstats = 0x40;
  OS.dcb.dbuf = filename;
  OS.dcb.dtimlo = 0x0F;
  OS.dcb.dbyt = 256;
  OS.dcb.daux1 = slot;
  OS.dcb.daux2 = 0;
  siov();
}

/**
 * Mount all hosts and devices
 */
unsigned char io_mount_all(void)
{
  OS.dcb.ddevic = 0x70;
  OS.dcb.dunit = 1;
  OS.dcb.dcomnd = 0xD7;
  OS.dcb.dstats = 0x00;
  OS.dcb.dbuf = 0x00;
  OS.dcb.dtimlo = 0x0F;
  OS.dcb.dbyt = 0;
  OS.dcb.daux = 0;
  siov();

  return OS.dcb.dstats; // 1 = successful, anything else = error.
}

#endif /* __ATARI__ */