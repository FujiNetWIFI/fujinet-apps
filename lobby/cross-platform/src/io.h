/**
 * @brief   Defines low level FujiNet IO function that must be implemented by each platform
 * @author  Eric Carr
 * @license gpl v. 3
 */

/* ******************************************************************** 
 * Common defs and vars to be used by each implementation.
 * ********************************************************************/

#ifndef IO_H
#define IO_H

#include <stdbool.h>

#define FILE_MAXLEN 36
#define SSID_MAXLEN 33 /* 32 + NULL */

#define FUJI_DEVICE_MODE_READ 1
#define FUJI_DEVICE_MODE_WRITE 2

#define MAX_HOST_LEN 32
#define NUM_HOST_SLOTS 8

typedef unsigned char HostSlot[32];

typedef struct {
  unsigned char hostSlot;
  unsigned char mode;
  unsigned char file[FILE_MAXLEN];
} DeviceSlot;

static HostSlot hostSlots[8];
static DeviceSlot deviceSlots[8];

/* ********************************************************************
 * Functions that must be implemented by each platform
 * ********************************************************************/

bool io_error(void);
unsigned char io_status(void);
void io_init(void);
void io_get_device_slots(DeviceSlot *d);
void io_get_host_slots(HostSlot *h);
void io_put_host_slots(HostSlot *h);
void io_put_device_slots(DeviceSlot *d);
void io_mount_host_slot(unsigned char hs);
void io_set_device_filename(unsigned char ds, char* e);
char *io_get_device_filename(unsigned char ds);
void io_mount_disk_image(unsigned char ds, unsigned char mode);
void io_set_boot_config(unsigned char toggle);
void io_umount_disk_image(unsigned char ds);
void io_boot(void);
bool io_get_device_enabled_status(unsigned char d);
void io_update_devices_enabled(bool *e);
void io_enable_device(unsigned char d);
void io_disable_device(unsigned char d);
unsigned char io_device_slot_to_device(unsigned char ds);
void io_get_filename_for_device_slot(unsigned char slot, const char* filename);
unsigned char io_mount_all(void);
void io_boot(void);

#endif /* IO_H */